#pragma once

#include <stdint.h>
#include "gdt_types.h"

namespace GDT
{
    class GDT
    {
        static constexpr const unsigned int GDT_SIZE = 6;

    public:
        GDT();
        void Setup();
        void AddDescriptor(unsigned int index, const gdt_descriptor_t& desc);

    private:
        gdt_descriptor_t m_gdt[GDT_SIZE];
        gdtr m_gdtr = {};
    };
}