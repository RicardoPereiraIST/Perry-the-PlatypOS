#pragma once

#include <stdint.h>

class A20Enabler
{
public:
    A20Enabler() = default;

    bool TryEnableA20() const;

private:
    bool IsA20Enabled() const;
    void TryEnableWithOutputPort() const;
    void TryEnableFast() const;
};