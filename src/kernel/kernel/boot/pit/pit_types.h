#pragma once

#include <stdint.h>

namespace PIT
{
    static constexpr const uint8_t OCW_MASK_BINCOUNT = 1;   	    //00000001
    static constexpr const uint8_t OCW_MASK_MODE = 0xE;		        //00001110
    static constexpr const uint8_t OCW_MASK_RL = 0x30;	            //00110000
    static constexpr const uint8_t OCW_MASK_COUNTER = 0xC0;	        //11000000

    //-----------------------------------------------
    //	Operational Command control bits
    //-----------------------------------------------

    // Use when setting binary count mode
    static constexpr const uint8_t OCW_BINCOUNT_BINARY = 0;		    //0
    static constexpr const uint8_t OCW_BINCOUNT_BCD = 1;	        //1

    // Use when setting counter mode
    static constexpr const uint8_t OCW_MODE_TERMINALCOUNT = 0;		//0000
    static constexpr const uint8_t OCW_MODE_ONESHOT = 0x2;		    //0010
    static constexpr const uint8_t OCW_MODE_RATEGEN = 0x4;		    //0100
    static constexpr const uint8_t OCW_MODE_SQUAREWAVEGEN = 0x6;	//0110
    static constexpr const uint8_t OCW_MODE_SOFTWARETRIG = 0x8;		//1000
    static constexpr const uint8_t OCW_MODE_HARDWARETRIG = 0xA;		//1010

    // Use when setting data transfer
    static constexpr const uint8_t OCW_RL_LATCH = 0;			    //000000
    static constexpr const uint8_t OCW_RL_LSBONLY = 0x10;		    //010000
    static constexpr const uint8_t OCW_RL_MSBONLY = 0x20;		    //100000
    static constexpr const uint8_t OCW_RL_DATA = 0x30;		        //110000

    // Use when setting the counter we are working with
    static constexpr const uint8_t OCW_COUNTER_0 = 0;		        //00000000
    static constexpr const uint8_t OCW_COUNTER_1 = 0x40;	        //01000000
    static constexpr const uint8_t OCW_COUNTER_2 = 0x80;	        //10000000
}