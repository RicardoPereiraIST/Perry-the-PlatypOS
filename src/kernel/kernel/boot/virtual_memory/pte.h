#pragma once

#include <stdint.h>

namespace Memory
{
    struct PTE
    {
        enum EFlags
        {
            PRESENT			=	1,			//0000000000000000000000000000001
            WRITABLE		=	2,			//0000000000000000000000000000010
            USER			=	4,			//0000000000000000000000000000100
            WRITETHOUGH		=	8,			//0000000000000000000000000001000
            NOT_CACHEABLE	=	0x10,		//0000000000000000000000000010000
            ACCESSED		=	0x20,		//0000000000000000000000000100000
            DIRTY			=	0x40,		//0000000000000000000000001000000
            PAT				=	0x80,		//0000000000000000000000010000000
            CPU_GLOBAL		=	0x100,		//0000000000000000000000100000000
            LV4_GLOBAL		=	0x200,		//0000000000000000000001000000000
            FRAME			=	0x7FFFF000 	//1111111111111111111000000000000
        };

        void AddAttr(uint32_t attr);
        void DeleteAttr(uint32_t attr);
        void SetFrame(uint32_t physicalAddr);
        bool IsPresent() const;
        bool IsWritable() const;
        uint32_t GetFrameAddr() const;

        uint32_t m_entry;
    };
}