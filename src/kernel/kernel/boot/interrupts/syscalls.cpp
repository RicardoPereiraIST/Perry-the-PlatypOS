#include "syscalls.h"
#include "idt_types.h"
#include "registers.h"
#include "../utils/globals.h"

#include <stdio.h>

namespace Interrupt
{
    void* SysCalls::s_sysCalls[MAX_SYSCALLS] = { reinterpret_cast<void*>(&printf) };

    void SysCalls::Handler(Registers* regs)
    {
        register int32_t eax asm("eax");

        if (eax >= MAX_SYSCALLS)
        {
            return;
        }

        void* pCall = s_sysCalls[0];

        int ret = 0;

        asm volatile ("push %%edi;\n\t"
            "push %%esi;\n\t"
            "push %%edx;\n\t"
            "push %%ecx;\n\t"
            "push %%ebx;\n\t"
            "call *%1;\n\t"
            "pop %%ebx;\n\t"
            "pop %%ecx;\n\t"
            "pop %%edx;\n\t"
            "pop %%esi;\n\t"
            "pop %%edi;\n\t"
            : "=a" (ret)
            : "r" (pCall)
            : "edi", "esi", "edx", "ecx", "ebx", "memory");
        
        regs->eax = ret;
    }

    void SysCalls::Setup()
    {
        selector_t selector{};
        flag_t flags{};

        selector.index = GDT::CODE_SELECTOR;

        flags.p = 1;
        flags.gate_type = INTERRUPT_GATE_TYPE;
        flags.dpl = 3;

        s_globals.IDT().AddDescriptor(0x80, &Handler, selector, flags);
    }
}