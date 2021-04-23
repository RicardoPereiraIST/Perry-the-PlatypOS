#include "types.h"
#include "interrupts.h"
#include <string.h>

namespace Interrupt
{
    IDT::IDT()
    {
        memset(m_idt, 0, sizeof(idt_descriptor_t) * SIZE);
    }


    void IDT::Setup()
    {
        // Init IDTs
        {
            selector_t selector;
            selector.ti = 0;
            selector.rpl = 0;
            selector.index = ::GDT::CODE_SELECTOR;

            flag_t flags;
            flags.gate_type = INTERRUPT_GATE_TYPE;
            flags.s = 0;
            flags.dpl = 0;
            flags.p = 1;

            for (uint16_t i = 0; i < SIZE; ++i)
            {
                idt_descriptor_t& entry = m_idt[i];

                entry.selector = selector;
                entry.zero = 0;
                entry.flags = flags;

                uintptr_t address = reinterpret_cast<uintptr_t>(m_irsTable[i]);
                entry.offset_low = address & 0xFFFF;
                entry.offset_high = (address & 0xFFFF0000) >> 16;
            }
        }

        // Init IDT Pointer
        m_idtr.base = reinterpret_cast<uint64_t>(m_idt);
        m_idtr.limit = SIZE * sizeof(idt_descriptor_t) - 1;

        // Load IDT
        asm volatile("lidt %0" : : "m" (m_idtr));
    }
};