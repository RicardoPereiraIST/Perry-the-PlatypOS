#pragma once

#include <string.h>
#include <sys/icxxabi.h>

namespace Commands
{
    struct ICommand
    {
        // Uncomment after figuring out to solve the bug: 'undefined reference to _ZdlPvm'
        // Without this, the code is incorrect
        // virtual ~ICommand() {};

        virtual const char* GetCommand() const = 0;
        virtual bool ExecuteCommand(const char* szBuffer) const
        {
            if (strcmp(szBuffer, GetCommand()) == 0)
            {
                ExecuteCommand_Impl(szBuffer);
                return true;
            }
            return false;
        }

    private:
        virtual void ExecuteCommand_Impl(const char* szBuffer) const = 0;
    };
}