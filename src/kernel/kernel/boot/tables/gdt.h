#pragma once

#include <stdint.h>
#include "types.h"

namespace GDT
{
    class GDT
    {
        static constexpr const unsigned int GDT_SIZE = 3;

    public:
        GDT();
        void Setup();

    private:
        gdt_descriptor_t m_gdt[GDT_SIZE];
        gdtr m_gdtr = {};
    };
}