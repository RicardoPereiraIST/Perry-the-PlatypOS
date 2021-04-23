#pragma once

#include "tss_table.h"

namespace GDT
{
    class GDT
    {
        static constexpr const unsigned int GDT_SIZE = 8;

    public:
        GDT();
        void Setup();

    private:
        gdt_descriptor_t m_gdt[GDT_SIZE];
        gdtr m_gdtr = {};
        CTssTable m_tss_table;
    };
}