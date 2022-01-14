#pragma once

#include <string.h>
#include <sys/icxxabi.h>

namespace Commands
{
    struct ICommand
    {
        virtual const char* GetCommand() const = 0;
        virtual bool ExecuteCommand(const char* szBuffer) const
        {
            if (strncmp(szBuffer, GetCommand(), strlen(GetCommand())) == 0)
            {
                ExecuteCommand_Impl(szBuffer);
                return true;
            }
            return false;
        }

    protected:
        // If virtual destructor is needed, one bug needs to be fixed first: 'undefined reference to _ZdlPvm'
        ~ICommand() = default;
    
    private:
        virtual void ExecuteCommand_Impl(const char* szBuffer) const = 0;
    };
}