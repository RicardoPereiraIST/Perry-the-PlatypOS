#pragma once

#include <stdint.h>
#include "../interrupts/registers.h"

namespace PIT
{
    class PIT
    {
    public:
        PIT() = default;

        void StartCounter(uint32_t freq, uint8_t counter, uint8_t mode);
        uint32_t GetTickCount() const;
        
    private:
        uint8_t ReadData(uint8_t counter) const;
        void SendData(uint8_t data, uint8_t counter) const;
        void SendCommand(uint8_t command) const;

        static void Handler(Registers* regs);
        static uint32_t s_ticks;
    };
}