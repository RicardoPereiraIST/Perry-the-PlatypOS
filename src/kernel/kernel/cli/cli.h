#pragma once

#include "../drivers/Keyboard/keyboard.h"
#include "Commands/ICommand.h"

#include "Commands/ClearTerminal.hpp"
#include "Commands/HelpCommand.hpp"

#include <stdint.h>

class Cli
{
    static constexpr const uint8_t COMMANDS = 2;
    static constexpr const uint16_t SIZE = 256;

public:
    Cli();
    ~Cli() = default;

    void Run();

    static Commands::ICommand* s_commands[COMMANDS];

private:
    void PromptUser() const;
    bool ExecuteCommand();

    Devices::Keyboard::Keycode GetChr() const;
    void GetCommand();

private:
    char m_buffer[SIZE];
};