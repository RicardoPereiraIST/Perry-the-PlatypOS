#include "irq.h"
#include "../globals.h"
#include "../helpers.h"

namespace Interrupt
{
    THandler IRQ::s_handlers[]{};

    static void SetMask(uint8_t i)
    {
        const uint16_t port = i < 8 ? 0 : 1;
        const uint8_t value = s_globals.PIC().ReadData(port) | (1 << i);
        s_globals.PIC().SendData(value, port);
    }

    static void ClearMask(uint8_t i)
    {
        const uint16_t port = i < 8 ? 0 : 1;
        const uint8_t value = s_globals.PIC().ReadData(port) & ~(1 << i);
        s_globals.PIC().SendData(value, port);
    }

    IRQ::IRQ()
    {}

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
        if (regs->int_no <= 47 && regs->int_no >= 32)
        {
            if (s_handlers[regs->int_no - 32])
            {
                s_handlers[regs->int_no - 32](regs);
            }
        }

        if (regs->int_no >= 0x40)
        {
            s_globals.PIC().SendCommand(I86_PIC_OCW2_MASK_EOI, 1);
        }

        //! always send end-of-interrupt to primary pic
        s_globals.PIC().SendCommand(I86_PIC_OCW2_MASK_EOI, 0);
    }
}