#include "tss_table.h"
#include "../gdt/gdt_types.h"
#include "../utils/globals.h"

namespace TSS
{
    TssTable::TssTable()
        : m_table{}
    {
    }

    void TssTable::SetStack(uint16_t kernelSS, uint16_t kernelESP)
    {
        m_table[m_curTss].ss0 = kernelSS;
	    m_table[m_curTss].esp0 = kernelESP;
    }

    void TssTable::Flush() const
    {
        // Load tss
        asm volatile("mov $0x2b, %%ax;\t\n"
            "ltr %%ax;\t\n"
            :
            :
            : "eax", "memory");
    }

    void TssTable::AddDescriptor(uint16_t kernelSS, uint16_t kernelESP)
    {
        ::GDT::gdt_descriptor_t descriptor;

        task_state_segment_t& tss = m_table[++m_curTss];

        const uint32_t base = reinterpret_cast<uint32_t>(&tss);
        const uint32_t limit = base + sizeof(task_state_segment_t);

        descriptor.base_low = base & 0xFFFFFF;
        descriptor.base_high = (base & 0xFF000000) >> 24;

        descriptor.limit_low = limit & 0xFFFF;
        descriptor.limit_high = (limit & 0xF0000) >> 16;

        descriptor.ex = 1;
        descriptor.dc = 0;
        descriptor.rw = 0;
        descriptor.ac = 1;

        descriptor.s = 0;
        descriptor.privl = 3;
        descriptor.pr = 1;
        descriptor.flag0 = 0;
        descriptor.flag1 = 0;
        descriptor.sz = 0;
        descriptor.gr = 0;

        s_globals.GDT().AddDescriptor(5, descriptor);

        // setup tss
        tss.ss0 = kernelSS;
	    tss.esp0 = kernelESP;

        tss.cs = 0x0b;
        tss.ss = 0x13;
        tss.ds = 0x13;
        tss.es = 0x13;
        tss.fs = 0x13;
        tss.gs = 0x13;

        Flush();
    }
}