#include "pit.h"
#include "pic.h"
#include "types.h"
#include "../helpers.h"
#include "../globals.h"

#define		I86_PIT_REG_COUNTER0		0x40
#define		I86_PIT_REG_COUNTER1		0x41
#define		I86_PIT_REG_COUNTER2		0x42
#define		I86_PIT_REG_COMMAND			0x43

uint32_t PIT::s_ticks = 0;

void PIT::Handler(Registers *regs)
{
	s_ticks++;
}

PIT::PIT()
{}

uint8_t PIT::ReadData(uint8_t counter) const
{
	const uint8_t port = (counter == I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 :
		((counter == I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

	return BootHelpers::InByte(port);
}

void PIT::SendData(uint8_t data, uint8_t counter) const
{
	const uint8_t port = (counter == I86_PIT_OCW_COUNTER_0) ? I86_PIT_REG_COUNTER0 :
		((counter == I86_PIT_OCW_COUNTER_1) ? I86_PIT_REG_COUNTER1 : I86_PIT_REG_COUNTER2);

	BootHelpers::OutByte(data, port);
}

void PIT::SendCommand(uint8_t command) const
{
	BootHelpers::OutByte(command, I86_PIT_REG_COMMAND);
}

uint32_t PIT::GetTickCount() const
{
    return s_ticks;
}

void PIT::StartCounter(uint32_t freq, uint8_t counter, uint8_t mode)
{
	if (freq == 0)
	{
		return;
	}

	const uint16_t divisor = uint16_t(1193181 / (uint16_t)freq);

	//! send operational command
	uint8_t ocw = 0;
	ocw = (ocw & ~I86_PIT_OCW_MASK_MODE) | mode;
	ocw = (ocw & ~I86_PIT_OCW_MASK_RL) | I86_PIT_OCW_RL_DATA;
	ocw = (ocw & ~I86_PIT_OCW_MASK_COUNTER) | counter;
	SendCommand(ocw);

	//! set frequency rate
	SendData(divisor & 0xff, 0);
	SendData((divisor >> 8) & 0xff, 0);

	//! reset tick count
	s_ticks = 0;

	s_globals.IRQ().Add(0, Handler);
}
