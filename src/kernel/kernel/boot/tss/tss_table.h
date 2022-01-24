#pragma once

#include <stdint.h>
#include "tss_types.h"

namespace GDT
{
    struct gdt_descriptor_t;
}

namespace TSS
{
    class TssTable
    {
        static constexpr const uint8_t SIZE = 1;

    public:
        TssTable();
        void AddDescriptor(uint16_t kernelSS, uint16_t kernelESP);
        void SetStack(uint16_t kernelSS, uint16_t kernelESP);
        void Flush() const;

    private:
        task_state_segment_t m_table[SIZE];
        int m_curTss = -1;
    };
}