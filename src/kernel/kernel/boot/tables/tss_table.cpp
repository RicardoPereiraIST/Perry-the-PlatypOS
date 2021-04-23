#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tss_table.h"

namespace GDT
{
    CTssTable::CTssTable()
        : m_cur_tss(0)
    {
        memset(m_table, 0, sizeof(task_state_segment_t) * SIZE);
    }

    gdt_descriptor_t CTssTable::CreateDescriptor(const uint16_t dataSeg)
    {
        task_state_segment_t& tss = m_table[m_cur_tss++];

        // setup tss
        tss.ss0 = dataSeg; // data segment -> third entry of gdt

        // Load tss
        asm volatile("ltr %0" : : "m" (tss));
        
        // Setup Tss Descriptor
        gdt_descriptor_t descriptor;

        const uint32_t base = reinterpret_cast<uint32_t>(&tss);
        const uint32_t limit = base + sizeof(task_state_segment_t) - 1;

        descriptor.base_low = base & 0xFFFFFF;
        descriptor.base_high = (base & 0xFF000000) >> 24;

        descriptor.ex = 1;
        descriptor.dc = 0;
        descriptor.rw = 0;
        descriptor.ac = 1;

        descriptor.limit_low = limit & 0xFFFF;
        descriptor.limit_high = (limit & 0xF0000) >> 16;
        descriptor.s = 0;
        descriptor.privl = 0;
        descriptor.pr = 1;
        descriptor.flag0 = 0;
        descriptor.flag1 = 0;
        descriptor.sz = 1;
        descriptor.gr = 0;

        return descriptor;
    }
}