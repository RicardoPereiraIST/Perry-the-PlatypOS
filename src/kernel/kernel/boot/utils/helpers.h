#pragma once

#include <stdint.h>
#include "../a20/a20_enabler.h"

namespace BootHelpers
{
    #define BIT(x) 1 << x;

    template <typename T, uint64_t N>
    char (&_ArraySizeHelper(T (&array)[N]))[N];

    #define ARRAY_COUNT(array) (sizeof(BootHelpers::_ArraySizeHelper(array)))

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
        A20Enabler a20Enabler;
        return a20Enabler.TryEnableA20();
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

    inline void UserMode()
    {
        asm volatile("cli;\n\t"
            "mov $0x23, %%ax;\n\t"
            "mov %%ax, %%ds;\n\t"
            "mov %%ax, %%es;\n\t"
            "mov %%ax, %%fs;\n\t"
            "mov %%ax, %%gs;\n\t"

            "mov %%esp, %%eax;\n\t"
            "pushl $0x23;\n\t"
            "pushl %%eax;\n\t"
            "pushf;\n\t"
            
            "pop %%eax;\n\t"
            "or $0x200, %%eax;\n\t"
            "push %%eax;\n\t"
            
            "pushl $0x1b;\n\t"
            "push $fake_jump%=;\n\t"
            "iret;\n\t"
            "fake_jump%=:\n\t"
        :
        : 
        : "eax", "memory");
    }
}