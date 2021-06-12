#pragma once

#include "idt.h"

namespace Interrupt
{
    constexpr const uint8_t NUM_ISRS = 48;
    
    class ISR
    {
        struct ISRS
        {
            uint8_t index;
            void (*stub)(Registers*);
        };

    public:
        ISR();
        void Setup();
        void Add(uint8_t i, THandler handler);

        static void Call(Registers* regs);

    private:
        static void Stub(Registers* regs);

        static THandler s_handlers[NUM_ISRS];
        static ISRS s_isrs[NUM_ISRS];
    };
}