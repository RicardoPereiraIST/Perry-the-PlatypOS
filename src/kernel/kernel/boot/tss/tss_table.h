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
        static constexpr unsigned int SIZE = 1;

    public:
        TssTable();
        ::GDT::gdt_descriptor_t CreateDescriptor(const uint16_t dataSeg);

    private:
        task_state_segment_t m_table[SIZE];
        unsigned int m_cur_tss;
    };
}