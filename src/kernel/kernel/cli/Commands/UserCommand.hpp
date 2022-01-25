#pragma once

#include "ICommand.h"
#include "../../boot/utils/globals.h"
#include "../../boot/utils/helpers.h"

extern "C" void jump_usermode();

namespace Commands
{
    struct UserCommand : public ICommand
    {
        virtual const char* GetCommand() const override 
        {
            return "user";
        }
        
        virtual void ExecuteCommand_Impl(const char* szBuffer) const override
        {
            (void) szBuffer;

            int stack = 0;
            
            asm volatile("mov %%esp, %0;" : "=r"(stack) : );

            s_globals.TSS().SetStack(0x10, stack);
            
            BootHelpers::UserMode();

            const char* szUserModeString = "\nWelcome to User Mode!\n";

            // Test user mode
            asm volatile("xor %%eax, %%eax;\n\t"
                "leal (%0), %%ebx;\n\t"
                "int $0x80;\n\t"
                :
                : "r" (szUserModeString)
                : "eax", "ebx", "memory"
            );

            while(1);
        }
    };
}