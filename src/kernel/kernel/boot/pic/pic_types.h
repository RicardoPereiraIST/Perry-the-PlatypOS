#pragma once

#include <stdint.h>

namespace PIC
{
    // The following devices use PIC 1 to generate interrupts
    static constexpr const uint8_t IRQ_TIMER = 0;
    static constexpr const uint8_t IRQ_KEYBOARD = 1;
    static constexpr const uint8_t IRQ_SERIAL2 = 3;
    static constexpr const uint8_t IRQ_SERIAL1 = 4;
    static constexpr const uint8_t IRQ_PARALLEL2 = 5;
    static constexpr const uint8_t IRQ_DISKETTE = 6;
    static constexpr const uint8_t IRQ_PARALLEL1 = 7;

    // The following devices use PIC 2 to generate interrupts
    static constexpr const uint8_t IRQ_CMOSTIMER = 0;
    static constexpr const uint8_t IRQ_CGARETRACE = 1;
    static constexpr const uint8_t IRQ_AUXILIARY = 4;
    static constexpr const uint8_t IRQ_FPU = 5;
    static constexpr const uint8_t IRQ_HDC = 6;

    //-----------------------------------------------
    //	Command words are used to control the devices
    //-----------------------------------------------

    // Command Word 2 bit masks. Use when sending commands
    static constexpr const uint8_t OCW2_MASK_L1 = 1;		//00000001
    static constexpr const uint8_t OCW2_MASK_L2 = 2;		//00000010
    static constexpr const uint8_t OCW2_MASK_L3 = 4;		//00000100
    static constexpr const uint8_t OCW2_MASK_EOI = 0x20;	//00100000
    static constexpr const uint8_t OCW2_MASK_SL = 0x40;	    //01000000
    static constexpr const uint8_t OCW2_MASK_ROTATE = 0x80;	//10000000

    // Command Word 3 bit masks. Use when sending commands
    static constexpr const uint8_t OCW3_MASK_RIS = 1;		//00000001
    static constexpr const uint8_t OCW3_MASK_RIR = 2;		//00000010
    static constexpr const uint8_t OCW3_MASK_MODE = 4;		//00000100
    static constexpr const uint8_t OCW3_MASK_SMM = 0x20;	//00100000
    static constexpr const uint8_t OCW3_MASK_ESMM = 0x40;	//01000000
    static constexpr const uint8_t OCW3_MASK_D7 = 0x80;     //10000000
}