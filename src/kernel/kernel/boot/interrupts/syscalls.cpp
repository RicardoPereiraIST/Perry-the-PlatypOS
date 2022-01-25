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
        if (regs->eax >= MAX_SYSCALLS)
        {
            return;
        }

        void* pCall = s_sysCalls[regs->eax];

        int ret = 0;

        asm volatile ("push %1;\n\t"
            "push %2;\n\t"
            "push %3;\n\t"
            "push %4;\n\t"
            "push %5;\n\t"
            "call *%6;\n\t"
            "pop %5;\n\t"
            "pop %4;\n\t"
            "pop %3;\n\t"
            "pop %2;\n\t"
            "pop %1;\n\t"
            : "=a" (ret)
            : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (pCall)
            :);
        
        regs->eax = ret;
    }

    void SysCalls::Setup()
    {
        flag_t flags{};
        flags.p = 1;
        flags.gate_type = INTERRUPT_GATE_TYPE;
        flags.dpl = 3;

        s_globals.ISR().Add(0x80, &Handler, flags);
    }
}