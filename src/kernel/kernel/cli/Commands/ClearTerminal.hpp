#pragma once

#include "ICommand.h"
#include <kernel/tty.h>

namespace Commands
{
    struct ClearTerminal : public ICommand
    {
        virtual const char* GetCommand() const override 
        {
            return "cls";
        }
        
        virtual void ExecuteCommand_Impl(const char* szBuffer) const override
        {
            (void) szBuffer;
            terminal_initialize();
        }
    };
}