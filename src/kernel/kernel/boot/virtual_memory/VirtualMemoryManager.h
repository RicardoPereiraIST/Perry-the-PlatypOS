#pragma once

#include <stdint.h>
#include "pte.h"
#include "pde.h"

namespace Memory
{
    class VirtualMemoryManager
    {
        static constexpr const uint32_t PAGES_PER_TABLE = 1024;
        static constexpr const uint32_t PAGES_PER_DIR = 1024;

        static constexpr const uint32_t PTABLE_ADDR_SPACE_SIZE = 0x400000;
        static constexpr const uint64_t DTABLE_ADDR_SPACE_SIZE = 0x100000000;
        static constexpr const uint32_t PAGE_SIZE = 4096;

        #define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
        #define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
        #define PAGE_GET_PHYSICAL_ADDRESS(x) (x ? (x->m_entry & ~0xfff) : 0)

        struct PageTable
        {
            void AddPage(uint32_t index, const PTE& page);
            PTE* GetEntry(uint32_t index);

            PTE m_entries[PAGES_PER_TABLE];
        };

        // page directory
        struct PageDirectory
        {
            PDE* GetEntry(uint32_t index);

            PDE m_entries[PAGES_PER_DIR];
        };

    public:
        void MapPage(void* physicalAddr, void* virtualAddr);
        bool Setup();
        bool AllocPage(PTE* entry);
        void FreePage(PTE* entry);
        bool SwitchDirectory(PageDirectory* directory);
        PageDirectory* GetDirectory() const;
        void FlushTlbEntry(uint32_t entry);
        PTE* LookupPageEntry(PageTable* table, uint32_t addr) const;
        PDE* LookupDirectoryEntry(PageDirectory* dir, uint32_t addr) const;

    private:
        PageDirectory* m_curDir = nullptr;
        uint32_t m_curPdbr = 0; // current page directory base register
    };
}