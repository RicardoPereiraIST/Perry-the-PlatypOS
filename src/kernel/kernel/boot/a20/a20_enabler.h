#pragma once

#include <stdint.h>

class A20Enabler
{
public:
    A20Enabler() = default;

    bool TryEnableA20() const;

private:
    bool IsA20Enabled() const;
    void TryEnableWithBios() const;
    void TryEnableWithKeyboardController() const;
    void TryEnableWithOutputPort() const;
    void TryEnableFast() const;

    uint8_t InByte(uint16_t port) const;
    void OutByte(uint8_t value, uint16_t port) const;
};