#include "irq.h"
#include "../pic/pic_types.h"
#include "../utils/globals.h"
#include "../utils/helpers.h"

namespace Interrupt
{
    THandler IRQ::s_handlers[]{};

    inline static void SetMask(uint8_t i)
    {
        const uint16_t port = i < 8 ? 0 : 1;
        const uint8_t value = s_globals.PIC().ReadData(port) | (1 << i);
        s_globals.PIC().SendData(value, port);
    }

    inline static void ClearMask(uint8_t i)
    {
        const uint16_t port = i < 8 ? 0 : 1;
        const uint8_t value = s_globals.PIC().ReadData(port) & ~(1 << i);
        s_globals.PIC().SendData(value, port);
    }

    void IRQ::Setup()
    {
        for (uint8_t i = 0; i < 16; i++)
        {
            s_globals.ISR().Add(32 + i, Stub);
        }
    }
    
    void IRQ::Add(uint8_t i, THandler handler)
    {
        BootHelpers::DisableInterrupts();
        s_handlers[i] = handler;
        ClearMask(i);
        BootHelpers::EnableInterrupts();
    }

    void IRQ::Stub(Registers* regs)
    {
        const uint32_t interrupt_number = regs->int_no;

        if (interrupt_number >= 32 && interrupt_number <= 47)
        {
            const uint32_t offset_interrupt_number = interrupt_number - 32;
            if (s_handlers[offset_interrupt_number])
            {
                s_handlers[offset_interrupt_number](regs);
            }
        }

        if (interrupt_number >= 0x40)
        {
            s_globals.PIC().SendCommand(PIC::OCW2_MASK_EOI, 1);
        }

        // Always send end-of-interrupt to primary pic
        s_globals.PIC().SendCommand(PIC::OCW2_MASK_EOI, 0);
    }
}