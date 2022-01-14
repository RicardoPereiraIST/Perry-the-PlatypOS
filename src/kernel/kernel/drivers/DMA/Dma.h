#pragma once

#include "DmaTypes.h"
#include <stdint.h>

namespace Devices
{
    class DMA
    {
    public:
        // Sets the mode of a channel
        void SetMode(uint8_t channel, uint8_t mode) const;

        // Prepares for generic channel read
        void SetRead(uint8_t channel) const;

        // Prepares for generic channel write
        void SetWrite(uint8_t channel) const;

        // Sets the address of a channel
        void SetAddress(uint8_t channel, uint8_t low, uint8_t high) const;

        // Sets the counter of a channel
        void SetCount(uint8_t channel, uint8_t low, uint8_t high) const;

        // Masks a channel
        void MaskChannel(uint8_t channel) const;

        // Unmasks a channel
        void UnmaskChannel(uint8_t channel) const;

        // Resets a flipflop
        void ResetFlipflop(int dma) const;

        // Reset the dma to defaults
        void Reset() const;

        // Sets an external page register
        void SetExternalPageRegister(uint8_t reg, uint8_t val) const;

        // Unmasks all registers
        void UnmaskAll() const;

    private:
    };
}