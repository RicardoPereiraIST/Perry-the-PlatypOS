#pragma once

#include <stdint.h>

struct multiboot_info;

namespace Memory
{
    class PhysicalMemoryManager
    {
        static constexpr const uint8_t BLOCKS_PER_BYTE = 8;
        static constexpr const uint32_t BLOCK_SIZE = 4096;
        static constexpr const uint32_t BLOCK_ALIGN = BLOCK_SIZE;

    public:
        PhysicalMemoryManager() = default;
        ~PhysicalMemoryManager() = default;

        bool Init(multiboot_info* pInfo, uint32_t start, uint32_t kernelSize);

        void* AllocBlock();
        void* AllocBlocks(uint32_t size);
        void FreeBlock(void* pAddr);
        void FreeBlocks(void* pAddr, uint32_t size);

        uint32_t GetSize() const;
        uint32_t GetBlocksCount() const;
        uint32_t GetBlockSize() const;
        uint32_t GetUsedBlocksCount() const;
        uint32_t GetFreeBlocksCount() const;

        bool IsPagingEnabled() const;
        void EnablePaging(bool enable);

        void LoadPDBR(uint32_t addr) const;

    private:
        // Sets region as free (unused)
        void InitRegion(uint32_t addr, uint32_t size);
        // Sets region as used
        void DeInitRegion(uint32_t addr, uint32_t size);

        void SetBit(uint32_t bit);
        void UnsetBit(uint32_t bit);
        bool TestBit(uint32_t bit) const;
        uint32_t FindFirstFreeFrame() const;
        uint32_t FindFirstFreeFrames(uint32_t size) const;

        uint32_t m_memorySize;
        uint32_t m_usedBlocks;
        uint32_t m_maxBlocks;
        uint32_t* m_pMemoryMap;
    };
}