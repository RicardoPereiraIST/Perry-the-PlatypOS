#include "Dma.h"
#include "../../boot/utils/helpers.h"

namespace Devices
{
    void DMA::SetMode(uint8_t channel, uint8_t mode) const
    {
        const int dma = (channel < 4) ? 0 : 1;
        const int chan = (dma == 0) ? channel : channel - 4;

        MaskChannel(channel);
        BootHelpers::OutByte(chan | (mode), (channel < 4) ? (uint16_t) DMA0_IO::DMA0_MODE_REG : (uint16_t) DMA1_IO::DMA1_MODE_REG);
        UnmaskAll();
    }

    void DMA::SetRead(uint8_t channel) const
    {
        SetMode(channel, DMA_MODE_REG_MASK::READ_TRANSFER | DMA_MODE_REG_MASK::TRANSFER_SINGLE | DMA_MODE_REG_MASK::MASK_AUTO);
    }

    void DMA::SetWrite(uint8_t channel) const
    {
        SetMode(channel, DMA_MODE_REG_MASK::WRITE_TRANSFER | DMA_MODE_REG_MASK::TRANSFER_SINGLE | DMA_MODE_REG_MASK::MASK_AUTO);
    }

    void DMA::SetAddress(uint8_t channel, uint8_t low, uint8_t high) const
    {
        if (channel > DMA_CHANNELS_PER_DMAC)
        {
            return;
        }

        unsigned short port = 0;
        switch (channel)
        {
            case 0:
            {
                port = DMA0_CHANNEL_IO::DMA0_CHAN0_ADDR_REG;
                break;
            }
            case 1:
            {
                port = DMA0_CHANNEL_IO::DMA0_CHAN1_ADDR_REG;
                break;
            }
            case 2:
            {
                port = DMA0_CHANNEL_IO::DMA0_CHAN2_ADDR_REG;
                break;
            }
            case 3:
            {
                port = DMA0_CHANNEL_IO::DMA0_CHAN3_ADDR_REG;
                break;
            }
            case 4:
            {
                port = DMA1_CHANNEL_IO::DMA1_CHAN4_ADDR_REG;
                break;
            }
            case 5:
            {
                port = DMA1_CHANNEL_IO::DMA1_CHAN5_ADDR_REG;
                break;
            }
            case 6:
            {
                port = DMA1_CHANNEL_IO::DMA1_CHAN6_ADDR_REG;
                break;
            }
            case 7:
            {
                port = DMA1_CHANNEL_IO::DMA1_CHAN7_ADDR_REG;
                break;
            }
        }

        BootHelpers::OutByte(low, port);
        BootHelpers::OutByte(high, port);
    }

    void DMA::SetCount(uint8_t channel, uint8_t low, uint8_t high) const
    {
        if (channel > DMA_CHANNELS_PER_DMAC)
        {
            return;
        }

        unsigned short port = 0;
        switch (channel)
        {
            case 0:
            {
                port = DMA0_CHANNEL_IO::DMA0_CHAN0_COUNT_REG;
                break;
            }
            case 1:
            {
                port = DMA0_CHANNEL_IO::DMA0_CHAN1_COUNT_REG;
                break;
            }
            case 2:
            {
                port = DMA0_CHANNEL_IO::DMA0_CHAN2_COUNT_REG;
                break;
            }
            case 3:
            {
                port = DMA0_CHANNEL_IO::DMA0_CHAN3_COUNT_REG;
                break;
            }
            case 4:
            {
                port = DMA1_CHANNEL_IO::DMA1_CHAN4_COUNT_REG;
                break;
            }
            case 5:
            {
                port = DMA1_CHANNEL_IO::DMA1_CHAN5_COUNT_REG;
                break;
            }
            case 6:
            {
                port = DMA1_CHANNEL_IO::DMA1_CHAN6_COUNT_REG;
                break;
            }
            case 7:
            {
                port = DMA1_CHANNEL_IO::DMA1_CHAN7_COUNT_REG;
                break;
            }
        }

        BootHelpers::OutByte(low, port);
        BootHelpers::OutByte(high, port);
    }

    void DMA::MaskChannel(uint8_t channel) const
    {
        if (channel <= 4)
        {
            BootHelpers::OutByte((1 << (channel-1)), DMA0_IO::DMA0_CHANMASK_REG);
        }
        else
        {
            BootHelpers::OutByte((1 << (channel-5)), DMA1_IO::DMA1_CHANMASK_REG);
        }
    }

    void DMA::UnmaskChannel(uint8_t channel) const
    {
        if (channel <= 4)
        {
            BootHelpers::OutByte(channel, DMA0_IO::DMA0_CHANMASK_REG);
        }
        else
        {
            BootHelpers::OutByte(channel, DMA1_IO::DMA1_CHANMASK_REG);
        }
    }

    void DMA::ResetFlipflop(int dma) const
    {
        if (dma > 2)
        {
            return;
        }

        //It doesn't matter what is written to this register
        BootHelpers::OutByte(0xff, ((dma == 0) ? (uint16_t) DMA0_IO::DMA0_CLEARBYTE_FLIPFLOP_REG : (uint16_t) DMA1_IO::DMA1_CLEARBYTE_FLIPFLOP_REG));
    }

    void DMA::Reset() const
    {
        // It doesn't matter what is written to this register
        BootHelpers::OutByte(0xff, DMA0_IO::DMA0_TEMP_REG);
    }

    void DMA::SetExternalPageRegister(uint8_t reg, uint8_t val) const
    {
        if (reg > 14)
        {
            return;
        }

        unsigned short port = 0;
        switch (reg)
        {
            case 1:
            {
                port = DMA0_PAGE_REG::CHAN1_ADDRBYTE2;
                break;
            }
            case 2:
            {
                port = DMA0_PAGE_REG::CHAN2_ADDRBYTE2;
                break;
            }
            case 3:
            {
                port = DMA0_PAGE_REG::CHAN3_ADDRBYTE2;
                break;
            }
            case 4:
            {
                // Nothing should ever write to register 4
                return;
            }
            case 5:
            {
                port = DMA0_PAGE_REG::CHAN5_ADDRBYTE2;
                break;
            }
            case 6:
            {
                port = DMA0_PAGE_REG::CHAN6_ADDRBYTE2;
                break;
            }
            case 7:
            {
                port = DMA0_PAGE_REG::CHAN7_ADDRBYTE2;
                break;
            }
        }

        BootHelpers::OutByte(val, port);
    }

    void DMA::UnmaskAll() const
    {
        // It doesn't matter what is written to this register
        BootHelpers::OutByte(0xff, DMA1_IO::DMA1_UNMASK_ALL_REG);
    }
}
