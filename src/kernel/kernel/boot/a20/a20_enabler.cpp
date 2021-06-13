#include "a20_enabler.h"
#include "../utils/helpers.h"

bool A20Enabler::TryEnableA20() const
{
    if (IsA20Enabled())
    {
        return true;
    }

    TryEnableWithOutputPort();

    if (IsA20Enabled())
    {
        return true;
    }

    TryEnableFast();

    return IsA20Enabled();
}

bool A20Enabler::IsA20Enabled() const
{
    asm volatile goto ("pushal\n\t"
                    "mov $0x112345, %%edi;\n\t"
                    "mov $0x012345, %%esi;\n\t"
                    "mov %%esi, (%%esi);\n\t"
                    "mov %%edi, (%%edi);\n\t"
                    "cmpsd;\n\t"
                    "popal;\n\t"
                    "jne %l[A20_on];"
                    :
                    :
                    : "esi", "edi", "memory"
                    : A20_on
                    );
    
    return false;

    A20_on:
        return true;
}

void A20Enabler::TryEnableWithOutputPort() const
{
    constexpr const uint32_t TIMEOUT = 8192;

    auto waitOutput = [&]() -> bool
    {
        // wait output buffer to be full
        // timeout if it takes too long -> assume controller method doesn't work
        uint32_t iteration = TIMEOUT;
        while (((BootHelpers::InByte(0x64) & 0x1) == 0))
        {
            if (--iteration > 0)
            {
                break;
            }
        }

        return ((BootHelpers::InByte(0x64) & 0x1) != 0);
    };

    auto waitInput = [&]() -> bool
    {
        // wait input buffer to be empty
        // timeout if it takes too long -> assume controller method doesn't work
        uint32_t iteration = TIMEOUT;
        // while (((BootHelpers::InByte(0x64) & 0x10) != 0) && (--iteration > 0));
        while ((BootHelpers::InByte(0x64) & 0x10) != 0)
        {
            if (--iteration > 0)
            {
                break;
            }
        }

        return ((BootHelpers::InByte(0x64) & 0x10) == 0);
    };

    if (!waitInput())
    {
        return;
    }

    // disable keyboard
    BootHelpers::OutByte(0xAD, 0x64);

    if (!waitInput())
    {
        return;
    }

    // send read output port command
    BootHelpers::OutByte(0xD0, 0x64);

    if (!waitOutput())
    {
        return;
    }

    // read input buffer and enable a20
    const uint8_t inputBuffer = BootHelpers::InByte(0x60) | 0x10;

    if (!waitInput())
    {
        return;
    }

    // send write output port command
    BootHelpers::OutByte(0xD1, 0x64);

    if (!waitInput())
    {
        return;
    }

    // write input buffer with new value
    BootHelpers::OutByte(inputBuffer, 0x60);

    // enable keyboard
    BootHelpers::OutByte(0xAE, 0x64);

    if (!waitInput())
    {
        return;
    }
}

void A20Enabler::TryEnableFast() const
{
    uint8_t inputBuffer = BootHelpers::InByte(0x92);
    if ((inputBuffer & 0x10) == 0)
    {
        inputBuffer |= 0x10;
        inputBuffer &= 0xFE;
        BootHelpers::OutByte(inputBuffer, 0x92);
    }
}