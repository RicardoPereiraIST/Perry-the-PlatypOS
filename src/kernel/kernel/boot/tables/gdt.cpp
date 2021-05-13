#include "gdt.h"

namespace GDT
{
    static gdt_descriptor_t CreateNullDescriptor()
    {
        gdt_descriptor_t descriptor = {};
        return descriptor;
    }

    static gdt_descriptor_t CreateCodeDescriptor()
    {
        gdt_descriptor_t descriptor;

        descriptor.base_low = 0;
        descriptor.base_high = 0;
        descriptor.limit_low = 0xFFFF;
        descriptor.limit_high = 0xF;

        descriptor.ac = 0;
        descriptor.rw = 1;
        descriptor.dc = 0;
        descriptor.ex = 1;
        descriptor.s = 1;
        descriptor.privl = 0;
        descriptor.pr = 1;

        descriptor.flag0 = 0;
        descriptor.flag1 = 0;
        descriptor.sz = 1;
        descriptor.gr = 1;

        return descriptor;
    }

    static gdt_descriptor_t CreateDataDescriptor()
    {
        gdt_descriptor_t descriptor;

        descriptor.base_low = 0;
        descriptor.base_high = 0;
        descriptor.limit_low = 0xFFFF;
        descriptor.limit_high = 0xF;

        descriptor.ac = 0;
        descriptor.rw = 1;
        descriptor.dc = 0;
        descriptor.ex = 0;
        descriptor.s = 1;
        descriptor.privl = 0;
        descriptor.pr = 1;

        descriptor.flag0 = 0;
        descriptor.flag1 = 0;
        descriptor.sz = 1;
        descriptor.gr = 1;

        return descriptor;
    }

    GDT::GDT()
        : m_gdt{}
    {}

    void GDT::Setup()
    {
        // Init GDT descriptor
        m_gdt[0] = CreateNullDescriptor();
        m_gdt[1] = CreateCodeDescriptor();
        m_gdt[2] = CreateDataDescriptor();
        // m_gdt[3] = m_tss_table.CreateDescriptor(2);

        // // Init the GDT Pointer
        m_gdtr.length = sizeof(m_gdt) - 1;
        m_gdtr.pointer = reinterpret_cast<uint32_t>(&m_gdt);

        // Load the GDT
        asm volatile("lgdt %0" : : "m" (m_gdtr));
    }
}