#pragma once

#include <stdint.h>

namespace PIC
{
    class PIC
    {
    public:
        PIC() = default;
        void Setup(uint8_t base0, uint8_t base1) const;

        void SendCommand(uint8_t command, uint8_t picNumber) const;
        uint8_t ReadData(uint8_t picNumber) const;
        void SendData(uint8_t data, uint8_t picNumber) const;
    };
}