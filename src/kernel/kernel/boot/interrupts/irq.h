#pragma once

#include "idt_types.h"

namespace Interrupt
{
    class IRQ
    {
    public:
        IRQ() = default;
        void Setup();
        void Add(uint8_t i, THandler handler);

    private:
        static void Stub(Registers* regs);

        static THandler s_handlers[32];
    };
}