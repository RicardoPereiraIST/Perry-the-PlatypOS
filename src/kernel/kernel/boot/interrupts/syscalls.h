#pragma once

#include <stdint.h>

struct Registers;

namespace Interrupt
{
    class SysCalls
    {
        static constexpr const uint32_t MAX_SYSCALLS = 1;

    public:
        void Setup();

    private:
        static void Handler(Registers* regs);

        static void* s_sysCalls[MAX_SYSCALLS];
    };
}