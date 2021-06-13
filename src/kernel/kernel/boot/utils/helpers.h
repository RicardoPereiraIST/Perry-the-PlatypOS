#pragma once

#include <stdint.h>
#include "globals.h"

namespace BootHelpers
{
    inline uint8_t InByte(uint16_t port)
    {
        uint8_t value;
        asm volatile("inb %1, %0" : "=a" (value) : "dN" (port));
        return value;
    }

    inline void OutByte(uint8_t value, uint16_t port)
    {
        asm volatile("outb %0, %1" : : "a" (value), "dN" (port));
    }

    inline void DisableInterrupts()
    {
        asm volatile("cli");
    }

    inline void EnableInterrupts()
    {
        asm volatile("sti");
    }

    inline bool TryEnableA20()
    {
        // A20Enabler a20Enabler;
        // return a20Enabler.TryEnableA20();
        return false;
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

    // Generate Interrupts
    template <unsigned char N>
    void GenerateInterrupt()
    {
        asm volatile ("int %0\n" : : "N"(N));
    }
}