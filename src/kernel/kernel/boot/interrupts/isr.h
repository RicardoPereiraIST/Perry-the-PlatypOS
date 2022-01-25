#pragma once

#include "idt_types.h"

namespace Interrupt
{
    constexpr const uint32_t NUM_ISRS = 256;
    
    class ISR
    {
    private:
        struct ISRS
        {
            uint8_t index;
            void (*stub)(Registers*);
        };

    public:
        ISR() = default;
        void Setup();
        void Add(uint32_t i, THandler handler);
        void Add(uint32_t i, THandler handler, const flag_t& flags);

        static void Call(Registers* regs);

    private:
        void SetupMissingISRs();

        static void Stub(Registers* regs);

        static THandler s_handlers[NUM_ISRS];
        static ISRS s_isrs[NUM_ISRS];
    };
}