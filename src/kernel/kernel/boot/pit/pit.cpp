#include "pit.h"
#include "pit_types.h"
#include "../utils/helpers.h"
#include "../utils/globals.h"

static constexpr const uint8_t REG_COUNTER0 = 0x40;
static constexpr const uint8_t REG_COUNTER1 = 0x41;
static constexpr const uint8_t REG_COUNTER2 = 0x42;
static constexpr const uint8_t REG_COMMAND = 0x43;

namespace PIT
{
	uint32_t PIT::s_ticks = 0;

	void PIT::Handler(Registers* regs)
	{
		(void) regs;
		s_ticks++;
	}

	uint8_t PIT::ReadData(uint8_t counter) const
	{
		const uint8_t port = (counter == OCW_COUNTER_0) ? REG_COUNTER0 :
			((counter == OCW_COUNTER_1) ? REG_COUNTER1 : REG_COUNTER2);

		return BootHelpers::InByte(port);
	}

	void PIT::SendData(uint8_t data, uint8_t counter) const
	{
		const uint8_t port = (counter == OCW_COUNTER_0) ? REG_COUNTER0 :
			((counter == OCW_COUNTER_1) ? REG_COUNTER1 : REG_COUNTER2);

		BootHelpers::OutByte(data, port);
	}

	void PIT::SendCommand(uint8_t command) const
	{
		BootHelpers::OutByte(command, REG_COMMAND);
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
		ocw = (ocw & ~OCW_MASK_MODE) | mode;
		ocw = (ocw & ~OCW_MASK_RL) | OCW_RL_DATA;
		ocw = (ocw & ~OCW_MASK_COUNTER) | counter;
		SendCommand(ocw);

		//! set frequency rate
		SendData(divisor & 0xff, 0);
		SendData((divisor >> 8) & 0xff, 0);

		//! reset tick count
		s_ticks = 0;

		s_globals.IRQ().Add(0, Handler);
	}
}