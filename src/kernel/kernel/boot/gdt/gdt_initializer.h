#pragma once

#include "tss_table.h"

namespace GDT
{
    class CGDT
    {
        static constexpr const unsigned int GDT_SIZE = 8;

    public:
        CGDT();
        void Setup();

    private:
        gdt_descriptor_t m_gdt[GDT_SIZE];
        gdtr m_gdtr = {};
        CTssTable m_tss_table;
    };
}