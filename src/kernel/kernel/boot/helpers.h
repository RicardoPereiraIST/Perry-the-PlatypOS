#pragma once

#include <stdint.h>
#include "a20/a20_enabler.h"
#include "globals.h"

namespace BootHelpers
{
    inline uint8_t InByte(uint16_t port)
    {
        uint8_t value;
        asm volatile("in %1, %0" : "=a" (value) : "dN" (port));
        return value;
    }

    inline void OutByte(uint8_t value, uint16_t port)
    {
        asm volatile("out %0, %1" : : "a" (value), "dN" (port));
    }

    inline void DisableInterrupts()
    {
        asm volatile("cli;");
    }

    inline void EnableInterrupts()
    {
        asm volatile("sti;");
    }

    inline bool TryEnableA20()
    {
        A20Enabler a20Enabler;
        return a20Enabler.TryEnableA20();
    }


    // Generate Interrupts
    template <unsigned char N>
    void GenerateInterrupt()
    {
        asm volatile ("int %0\n" : : "N"(N));
    }

    // Self-modifying version
    inline void GenerateInterrupt(unsigned char n)
    {
        asm volatile("movb (1f+1), %0;\n\t"
                    "jmp 1f;\n\t"
                    "1:\n\t"
                    "int $0;"
                    :
                    : "r" (n)
                    );
    }

    template<typename T, typename R>
    void* void_cast(R(T::*f)())
    {
        union
        {
            R(T::*pf)();
            void* p;
        };
        pf = f;
        return p;
    }

    inline void InterruptDone(unsigned int interruptNumber)
    {
        //! insure its a valid hardware irq
        if (interruptNumber > 16)
        {
            return;
        }

        //! test if we need to send end-of-interrupt to second pic
        if (interruptNumber >= 8)
        {
            s_globals.PIC().SendCommand(I86_PIC_OCW2_MASK_EOI, 1);
        }

        //! always send end-of-interrupt to primary pic
        s_globals.PIC().SendCommand(I86_PIC_OCW2_MASK_EOI, 0);
    }
}