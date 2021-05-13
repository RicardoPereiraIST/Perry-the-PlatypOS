#include "pit.h"
#include "pic.h"
#include "types.h"
#include "../helpers.h"
#include "../globals.h"

#define		I86_PIT_REG_COUNTER0		0x40
#define		I86_PIT_REG_COUNTER1		0x41
#define		I86_PIT_REG_COUNTER2		0x42
#define		I86_PIT_REG_COMMAND			0x43

void PIT::Irq()
{
	m_ticks++;
	BootHelpers::InterruptDone(0);
}

PIT::PIT()
    : m_ticks(0)
{}

void PIT::Setup() const
{
	Interrupt::idt_descriptor_t descriptor;

	const uintptr_t base = reinterpret_cast<uintptr_t>(BootHelpers::void_cast(&PIT::Irq));
	descriptor.offset_low =	uint16_t(base & 0xffff);
	descriptor.offset_high = uint16_t((base >> 16) & 0xffff);
	descriptor.zero	= 0;
	descriptor.flags.gate_type = Interrupt::INTERRUPT_GATE_TYPE;
	descriptor.flags.dpl = 0;
	descriptor.flags.p = 1;
	descriptor.flags.s = 0;
	descriptor.selector.index = GDT::CODE_SELECTOR;
	descriptor.selector.ti = 0;
	descriptor.selector.rpl = 0;

	s_globals.IDT().AddDescriptor(32, descriptor);
}

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

uint32_t PIT::SetTickCount(uint32_t tickCount)
{
    const uint32_t curTickCount = m_ticks;
    m_ticks = tickCount;
    return curTickCount;
}

uint32_t PIT::GetTickCount() const
{
    return m_ticks;
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
	m_ticks = 0;
}
