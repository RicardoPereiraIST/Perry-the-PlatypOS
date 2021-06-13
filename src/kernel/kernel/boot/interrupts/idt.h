#pragma once

#include <stdint.h>
#include "idt_types.h"

namespace Interrupt
{    
    class IDT
    {
        static constexpr uint16_t SIZE = 256;
    public:
        IDT();
        void Setup();

        bool AddDescriptor(uint8_t i, THandler handler, selector_t selector, flag_t flags);

    private:
        idt_descriptor_t m_idt[SIZE];
        idtr m_idtr;
    };
}