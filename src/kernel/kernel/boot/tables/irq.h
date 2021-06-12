#pragma once

#include "idt.h"

namespace Interrupt
{
    class IRQ
    {
    public:
        IRQ();
        void Setup();
        void Add(uint8_t i, THandler handler);

    private:
        static void Stub(Registers* regs);

        static THandler s_handlers[32];
    };
}