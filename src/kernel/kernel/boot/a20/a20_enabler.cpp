#include "a20_enabler.h"

bool A20Enabler::TryEnableA20() const
{
    if (IsA20Enabled())
    {
        return true;
    }

    TryEnableWithBios();

    if (IsA20Enabled())
    {
        return true;
    }

    TryEnableWithKeyboardController();

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
                    "mov 0x112345, %%edi;\n\t"
                    "mov 0x012345, %%esi;\n\t"
                    "mov %%esi, (%%esi);\n\t"
                    "mov %%edi, (%%edi);\n\t"
                    "cmpsd;\n\t"
                    "popal;\n\t"
                    "jne %l[A20_on];"
                    :
                    :
                    : "esi", "edi"
                    : A20_on
                    );
    
    return false;

    A20_on:
        return true;
}

void A20Enabler::TryEnableWithBios() const
{
    asm volatile goto("pushal;\n\t"
                "mov $0x2043, %%ax;\n\t"
                "int $0x15;\n\t"
                "jb %l[early_return];\n\t"
                "cmpb $0x0, %%ah;\n\t"
                "jnz %l[early_return];\n\t"
                "mov $0x2402, %%ax;\n\t"
                "int $0x15;\n\t"
                "jb %l[early_return];\n\t"
                "cmpb $0x0, %%ah;\n\t"
                "jnz %l[early_return];\n\t"
                "cmpb $0x1, %%al;\n\t"
                "jz %l[early_return];\n\t"
                "mov $0x2401, %%ax;\n\t"
                "int $0x15;\n\t"
                "jb %l[early_return];\n\t"
                "cmpb $0x0, %%ah;\n\t"
                "jnz %l[early_return];\n\t"
                "popal;\n\t"
                :
                :
                : "eax"
                : early_return
                );

    early_return:
        return;
}

void A20Enabler::TryEnableWithOutputPort() const
{
    constexpr const uint32_t TIMEOUT = 8192;

    auto waitOutput = [&]() -> bool
    {
        // wait output buffer to be full
        // timeout if it takes too long -> assume controller method doesn't work
        uint32_t iteration = TIMEOUT;
        while (((InByte(0x64) & 0x1) == 0) && (--iteration > 0));

        return ((InByte(0x64) & 0x1) == 0);
    };

    auto waitInput = [&]() -> bool
    {
        // wait input buffer to be empty
        // timeout if it takes too long -> assume controller method doesn't work
        uint32_t iteration = TIMEOUT;
        while (((InByte(0x64) & 0x10) != 0) && (--iteration > 0));

        return ((InByte(0x64) & 0x10) != 0);
    };

    if (!waitInput())
    {
        return;
    }

    // disable keyboard
    OutByte(0xAD, 0x64);

    if (!waitInput())
    {
        return;
    }

    // send read output port command
    OutByte(0xD0, 0x64);

    if (!waitOutput())
    {
        return;
    }

    // read input buffer and enable a20
    const uint8_t inputBuffer = InByte(0x60) | 0x10;

    if (!waitInput())
    {
        return;
    }

    // send write output port command
    OutByte(0xD1, 0x64);

    if (!waitInput())
    {
        return;
    }

    // write input buffer with new value
    OutByte(inputBuffer, 0x60);

    // enable keyboard
    OutByte(0xAE, 0x64);

    if (!waitInput())
    {
        return;
    }
}

void A20Enabler::TryEnableWithKeyboardController() const
{
    OutByte(0xDD, 0x64);
}

void A20Enabler::TryEnableFast() const
{
    uint8_t inputBuffer = InByte(0x92);
    if ((inputBuffer & 0x10) != 0)
    {
        inputBuffer |= 0x10;
        inputBuffer &= 0xFE;
        OutByte(inputBuffer, 0x92);
    }
}

uint8_t A20Enabler::InByte(uint16_t port) const
{
    uint8_t value;
    asm volatile("in %1, %0" : "=a" (value) : "dN" (port));
    return value;
}

void A20Enabler::OutByte(uint8_t value, uint16_t port) const
{
    asm volatile("out %0, %1" : : "a" (value), "dN" (port));
}