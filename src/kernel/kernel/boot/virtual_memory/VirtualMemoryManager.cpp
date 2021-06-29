#include "VirtualMemoryManager.h"
#include "../utils/globals.h"
#include <string.h>

namespace Memory
{
    PTE* VirtualMemoryManager::PageTable::GetEntry(uint32_t index)
    {
        if (index < PAGES_PER_TABLE)
        {
            return &m_entries[index];
        }
        return nullptr;
    }

    void VirtualMemoryManager::PageTable::AddPage(uint32_t index, const PTE& page)
    {
        if (index < PAGES_PER_TABLE)
        {
            m_entries[index] = page;
        }
    }

    PDE* VirtualMemoryManager::PageDirectory::GetEntry(uint32_t index)
    {
        if (index < PAGES_PER_DIR)
        {
            return &m_entries[index];
        }
        return nullptr;
    }

    void VirtualMemoryManager::MapPage(void* physicalAddr, void* virtualAddr)
    {
        const PDE* pPDEntry = LookupDirectoryEntry(m_curDir, reinterpret_cast<uint32_t>(virtualAddr));
        if (pPDEntry && !pPDEntry->IsPresent())
        {
            PageTable* pTable = reinterpret_cast<PageTable*>(s_globals.PhysicalMemoryManager().AllocBlock());

            if (!pTable)
            {
                return;
            }

            memset(pTable, 0, sizeof(pTable));

            if (PDE* pNewEntry = LookupDirectoryEntry(m_curDir, reinterpret_cast<uint32_t>(virtualAddr)))
            {
                pNewEntry->AddAttr(PTE::PRESENT);
                pNewEntry->AddAttr(PTE::WRITABLE);
                pNewEntry->SetFrame(reinterpret_cast<uint32_t>(pTable));
            }
        }

        if (PageTable* pTable = reinterpret_cast<PageTable*>(PAGE_GET_PHYSICAL_ADDRESS(pPDEntry)))
        {
            if (PTE* pTableEntry = LookupPageEntry(pTable, reinterpret_cast<uint32_t>(virtualAddr)))
            {
                pTableEntry->SetFrame(reinterpret_cast<uint32_t>(physicalAddr));
                pTableEntry->AddAttr(PTE::PRESENT);
            }
        }
    }

    bool VirtualMemoryManager::Setup()
    {
        constexpr const uint32_t higherKernelAddr = 0xc0000000;

        PageTable* pTable = reinterpret_cast<PageTable*>(s_globals.PhysicalMemoryManager().AllocBlock());
        if (!pTable)
        {
            return false;
        }

        PageTable* pHigherHalfTable = reinterpret_cast<PageTable*>(s_globals.PhysicalMemoryManager().AllocBlock());
        if (!pHigherHalfTable)
        {
            return false;
        }

        memset(pTable, 0, sizeof(pTable));
        memset(pHigherHalfTable, 0, sizeof(pHigherHalfTable));

        for (uint32_t i = 0, frame = 0, virtualAddr = 0; i < PAGES_PER_TABLE; ++i, frame+=PAGE_SIZE, virtualAddr+=PAGE_SIZE)
        {
            PTE page{};
            page.AddAttr(PTE::PRESENT);
            page.SetFrame(frame);

            pHigherHalfTable->AddPage(PAGE_TABLE_INDEX(virtualAddr), page);
        }

        for (uint32_t i = 0, frame = 0x100000, virtualAddr = higherKernelAddr; i < PAGES_PER_TABLE; ++i, frame+=PAGE_SIZE, virtualAddr+=PAGE_SIZE)
        {
            PTE page{};
            page.AddAttr(PTE::PRESENT);
            page.SetFrame(frame);

            pTable->AddPage(PAGE_TABLE_INDEX(virtualAddr), page);
        }

        if (PageDirectory* pDir = reinterpret_cast<PageDirectory*>(s_globals.PhysicalMemoryManager().AllocBlocks(3)))
        {
            memset(pDir, 0, sizeof(pDir));

            if (PDE* pHigherHalfEntry = LookupDirectoryEntry(pDir, higherKernelAddr))
            {
                pHigherHalfEntry->AddAttr(PDE::PRESENT);
                pHigherHalfEntry->AddAttr(PDE::WRITABLE);
                pHigherHalfEntry->SetFrame(reinterpret_cast<uint32_t>(pTable));
            }

            if (PDE* pEntry = LookupDirectoryEntry(pDir, 0x0))
            {
                pEntry->AddAttr(PDE::PRESENT);
                pEntry->AddAttr(PDE::WRITABLE);
                pEntry->SetFrame(reinterpret_cast<uint32_t>(pHigherHalfTable));
            }

            SwitchDirectory(pDir);
            s_globals.PhysicalMemoryManager().EnablePaging(true);

            return true;
        }

        return false;
    }
    
    bool VirtualMemoryManager::AllocPage(PTE* pEntry)
    {
        if (pEntry)
        {
            if (void* pBlock = s_globals.PhysicalMemoryManager().AllocBlock())
            {
                pEntry->SetFrame(reinterpret_cast<uint32_t>(pBlock));
                pEntry->AddAttr(PTE::PRESENT);
                return true;
            }
        }
        
        return false;
    }
    
    void VirtualMemoryManager::FreePage(PTE* pEntry)
    {
        if (pEntry)
        {
            if (void* pAddr = reinterpret_cast<void*>(pEntry->GetFrameAddr()))
            {
                s_globals.PhysicalMemoryManager().FreeBlock(pAddr);
                pEntry->DeleteAttr(PTE::PRESENT);
            }            
        }
    }
    
    bool VirtualMemoryManager::SwitchDirectory(PageDirectory* pDirectory)
    {
        if (!pDirectory)
        {
            return false;
        }

        m_curDir = pDirectory;
        s_globals.PhysicalMemoryManager().LoadPDBR(reinterpret_cast<uint32_t>(m_curDir));
        return true;
    }
    
    VirtualMemoryManager::PageDirectory* VirtualMemoryManager::GetDirectory() const
    {
        return m_curDir;
    }
    
    void VirtualMemoryManager::FlushTlbEntry(uint32_t entry)
    {
        asm volatile("cli;\n\t"
                    "invlpg (%0);\n\t"
                    "sti;\n\t"
                    :
                    : "r"(entry)
                    : "memory"
        );
    }
    
    PTE* VirtualMemoryManager::LookupPageEntry(PageTable* pTable, uint32_t addr) const
    {
        if (pTable)
        {
            return pTable->GetEntry(PAGE_TABLE_INDEX(addr));
        }
        return nullptr;
    }
    
    PDE* VirtualMemoryManager::LookupDirectoryEntry(PageDirectory* pDir, uint32_t addr) const
    {
        if (pDir)
        {
            return pDir->GetEntry(PAGE_DIRECTORY_INDEX(addr));
        }
        return nullptr;
    }
}