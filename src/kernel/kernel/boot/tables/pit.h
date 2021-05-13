#pragma once

#include <stdint.h>

#define		I86_PIT_OCW_MASK_BINCOUNT		1		//00000001
#define		I86_PIT_OCW_MASK_MODE			0xE		//00001110
#define		I86_PIT_OCW_MASK_RL				0x30	//00110000
#define		I86_PIT_OCW_MASK_COUNTER		0xC0	//11000000

//-----------------------------------------------
//	Operational Command control bits
//-----------------------------------------------

//! Use when setting binary count mode
#define		I86_PIT_OCW_BINCOUNT_BINARY		0		//0
#define		I86_PIT_OCW_BINCOUNT_BCD		1		//1

//! Use when setting counter mode
#define		I86_PIT_OCW_MODE_TERMINALCOUNT	0		//0000
#define		I86_PIT_OCW_MODE_ONESHOT		0x2		//0010
#define		I86_PIT_OCW_MODE_RATEGEN		0x4		//0100
#define		I86_PIT_OCW_MODE_SQUAREWAVEGEN	0x6		//0110
#define		I86_PIT_OCW_MODE_SOFTWARETRIG	0x8		//1000
#define		I86_PIT_OCW_MODE_HARDWARETRIG	0xA		//1010

//! Use when setting data transfer
#define		I86_PIT_OCW_RL_LATCH			0			//000000
#define		I86_PIT_OCW_RL_LSBONLY			0x10		//010000
#define		I86_PIT_OCW_RL_MSBONLY			0x20		//100000
#define		I86_PIT_OCW_RL_DATA				0x30		//110000

//! Use when setting the counter we are working with
#define		I86_PIT_OCW_COUNTER_0			0		//00000000
#define		I86_PIT_OCW_COUNTER_1			0x40	//01000000
#define		I86_PIT_OCW_COUNTER_2			0x80	//10000000

namespace Interrupt
{
    class IDT;
}

class PIC;

class PIT
{
public:
    PIT();
    void Setup() const;

    void StartCounter(uint32_t freq, uint8_t counter, uint8_t mode);
    uint32_t GetTickCount() const;

    uint8_t ReadData(uint8_t counter) const;
    void SendData(uint8_t data, uint8_t counter) const;
    void SendCommand(uint8_t command) const;
    
private:
    uint32_t SetTickCount(uint32_t tickCount);

    void InterruptDone(unsigned int interruptNumber);
    void Irq();

    uint32_t m_ticks;
};