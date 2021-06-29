#include "PhysicalMemoryManager.h"
#include <string.h>
#include "../utils/multiboot.h"
#include <stdio.h>

namespace Memory
{
    static uint32_t CalculateMemSize(multiboot_info_t* pInfo)
    {
        uint32_t memSize = 0;
        for(uint32_t i = 0; i < pInfo->mmap_length; i += sizeof(multiboot_memory_map_t)) 
        {
            multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (pInfo->mmap_addr + i);
    
            if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
            {
                memSize += mmmt->len;
            }
        }

        return memSize;
    }

    bool PhysicalMemoryManager::Init(multiboot_info_t* pInfo, uint32_t start, uint32_t kernelSize)
    {
        if(!(pInfo->flags >> 6 & 0x1))
        {
            return false;
        }

        m_memorySize = CalculateMemSize(pInfo);
        m_pMemoryMap = (uint32_t*)(start + (kernelSize * 512));
        m_maxBlocks = GetSize() / BLOCK_SIZE;
        m_usedBlocks = m_maxBlocks;

        memset(m_pMemoryMap, 0xf, GetBlocksCount() / BLOCKS_PER_BYTE);

        // Alloc regions
        for(uint32_t i = 0; i < pInfo->mmap_length; i += sizeof(multiboot_memory_map_t)) 
        {
            const multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (pInfo->mmap_addr + i); 
            if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
            {
                InitRegion(mmmt->addr, mmmt->len);
            }
        }

        // Free kernel region
        DeInitRegion(start, kernelSize * 512);

        return true;
    }

    void PhysicalMemoryManager::InitRegion(uint32_t addr, uint32_t size)
    {
        uint32_t align = addr / BLOCK_SIZE;

        for (int blocks = size / BLOCK_SIZE; blocks > -1; --blocks)
        {
            UnsetBit(align++);
            --m_usedBlocks;
        }

        SetBit(0);
    }

    void PhysicalMemoryManager::DeInitRegion(uint32_t addr, uint32_t size)
    {
        uint32_t align = addr / BLOCK_SIZE;

        for (int blocks = size / BLOCK_SIZE; blocks > -1; --blocks)
        {
            UnsetBit(align++);
            ++m_usedBlocks;
        }
    }

    void* PhysicalMemoryManager::AllocBlock()
    {
        if (GetFreeBlocksCount() == 0)
        {
            return nullptr;
        }

        const int frame = FindFirstFreeFrame();
        if (frame == -1)
        {
            return nullptr;
        }

        SetBit(frame);
        uint32_t addr = frame * BLOCK_SIZE;
        ++m_usedBlocks;

        return (void*)addr;
    }

    void* PhysicalMemoryManager::AllocBlocks(uint32_t size)
    {
        if (GetFreeBlocksCount() <= size)
        {
            return nullptr;
        }

        const int frame = FindFirstFreeFrames(size);
        if (frame == -1)
        {
            return nullptr;
        }

        for (uint32_t i = 0; i < size; ++i)
        {
            SetBit(frame + i);
        }

        uint32_t addr = frame * BLOCK_SIZE;
        m_usedBlocks += size;

        return (void*)addr;
    }

    void PhysicalMemoryManager::FreeBlock(void* pAddr)
    {
        const uint32_t addr = (uint32_t)(pAddr);
        const int frame = addr / BLOCK_SIZE;
        UnsetBit(frame);
        --m_usedBlocks;
    }

    void PhysicalMemoryManager::FreeBlocks(void* pAddr, uint32_t size)
    {
        const uint32_t addr = (uint32_t)(pAddr);
        const int frame = addr / BLOCK_SIZE;

        for (uint32_t i = 0; i < size; ++i)
        {
            UnsetBit(frame + i);
        }

        m_usedBlocks -= size;
    }

    uint32_t PhysicalMemoryManager::GetSize() const
    {
        return m_memorySize;
    }

    uint32_t PhysicalMemoryManager::GetBlocksCount() const
    {
        return m_maxBlocks;
    }

    uint32_t PhysicalMemoryManager::GetBlockSize() const
    {
        return BLOCK_SIZE;
    }

    uint32_t PhysicalMemoryManager::GetUsedBlocksCount() const
    {
        return m_usedBlocks;
    }

    uint32_t PhysicalMemoryManager::GetFreeBlocksCount() const
    {
        return m_maxBlocks - m_usedBlocks;
    }

    bool PhysicalMemoryManager::IsPagingEnabled() const
    {
        uint32_t res;
        asm volatile("mov %%cr0, %0"
                    : "=r"(res)
                    :
                    :
                    );

        return (res & (1 << 31));
    }

    void PhysicalMemoryManager::EnablePaging(bool enable)
    {
        if (enable)
        {
            asm volatile("mov %%cr0, %%eax;\n\t"
                        "or $0x80000000, %%eax;\n\t"
                        "mov %%eax, %%cr0;\n\t"
                        :
                        :
                        : "eax", "memory"
                        );
        }
        else
        {
            asm volatile("mov %%cr0, %%eax;\n\t"
                        "and $0x7FFFFFFF, %%eax;\n\t"
                        "mov %%eax, %%cr0;\n\t"
                        :
                        :
                        : "eax", "memory"
                        );
        }
    }

    void PhysicalMemoryManager::LoadPDBR(uint32_t addr) const
    {
        asm volatile("mov %0, %%cr3;"
                    :
                    : "r"(addr)
                    : "memory"
                    );
    }

    void PhysicalMemoryManager::SetBit(uint32_t bit)
    {
        m_pMemoryMap[bit / 32] |= (1 << (bit % 32));
    }

    void PhysicalMemoryManager::UnsetBit(uint32_t bit)
    {
        m_pMemoryMap[bit / 32] &= ~(1 << (bit % 32));
    }

    bool PhysicalMemoryManager::TestBit(uint32_t bit) const
    {
        return m_pMemoryMap[bit / 32] & (1 << (bit % 32));
    }

    uint32_t PhysicalMemoryManager::FindFirstFreeFrame() const
    {
        const uint32_t maxIndex = GetBlocksCount() / 32;
        for (uint32_t i = 0; i < maxIndex; ++i)
        {
            if (m_pMemoryMap[i] != 0xFFFFFFFF)
            {
                for (uint32_t j = 0; j < 32; ++j)
                {
                    const int bit = 1 << j;
                    if (!(m_pMemoryMap[i] & bit))
                    {
                        return (i * 32) + j;
                    }
                }
            }
        }

        return -1;
    }

    uint32_t PhysicalMemoryManager::FindFirstFreeFrames(uint32_t size) const
    {
        if (size == 0)
        {
            return -1;
        }

        if (size == 1)
        {
            return FindFirstFreeFrame();
        }

        const uint32_t maxIndex = GetBlocksCount() / 32;
        for (uint32_t i = 0; i < maxIndex; ++i)
        {
            if (m_pMemoryMap[i] != 0xFFFFFFFF)
            {
                for (uint32_t j = 0; j < 32; ++j)
                {
                    const int bit = 1 << j;
                    if (!(m_pMemoryMap[i] & bit))
                    {
                        const int start = i * 32 + bit;
                        uint32_t free = 0;

                        for (uint32_t count = 0; count < size; ++count)
                        {
                            if (!TestBit(start + count))
                            {
                                ++free;
                            }

                            if (free == size)
                            {
                                return i * 32 + j;
                            }
                        }
                    }
                }
            }
        }

        return -1;
    }
}
