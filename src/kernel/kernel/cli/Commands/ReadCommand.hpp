#pragma once

#include "ICommand.h"
#include <stdio.h>
#include <stdint.h>

namespace Commands
{
    struct ReadCommand : public ICommand
    {
        static constexpr const uint8_t COMMAND_LEN = 4;

        virtual const char* GetCommand() const override 
        {
            return "read";
        }
        
        virtual void ExecuteCommand_Impl(const char* szBuffer) const override
        {
            uint32_t sectornum = 0;
            uint8_t* sectorData = 0;

            const size_t len = strlen(szBuffer);
            if (len > COMMAND_LEN + 1)
            {
                const char* szParameters = szBuffer + COMMAND_LEN + 1;
                sectornum = atoi(szParameters);
            }

            printf("\nSector %d contents:\n\n\r", sectornum);

            // read sector from disk
            sectorData = s_globals.FloppyDisk().ReadSector(sectornum);

            // display sector
            if (sectorData)
            {
                int i = 0;
                for (int c = 0; c < 4; ++c)
                {
                    for (int j = 0; j < 128; ++j)
                    {
                        printf("0x%x ", sectorData[i + j]);
                    }

                    i += 128;
                }
                printf("\n");
            }
            else
            {
                printf("\n\r*** Error reading sector from disk\n");
            }
        }
    };
}