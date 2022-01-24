#pragma once

#include "ICommand.h"
#include <stdio.h>

namespace Commands
{
    struct HelpCommand : public ICommand
    {
        virtual const char* GetCommand() const override 
        {
            return "help";
        }
        
        virtual void ExecuteCommand_Impl(const char* szBuffer) const override
        {
            (void) szBuffer;

            printf("\n");
            printf("Perry Os\n");
            printf("Supported commands:\n");
            printf("exit: quits and halts the system\n");
            printf("cls: clears the display\n");
            printf("read [x]: reads given sector (default = 0)\n");
            printf("user: goes into user mode\n");
            printf("help: displays this message\n");
        }
    };
}