#pragma once

#include <stdint.h>
#include "../drivers/Keyboard/keyboard.h"

class Cli
{
    static constexpr const uint32_t SIZE = 256;
public:
    Cli() = default;
    ~Cli() = default;

    void Run();
private:
    void PromptUser() const;
    bool ExecuteCommand();

    Devices::Keyboard::Keycode GetChr() const;
    void GetCommand();

private:
    char m_buffer[SIZE];
};