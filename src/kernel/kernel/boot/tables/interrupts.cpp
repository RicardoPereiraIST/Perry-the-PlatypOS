#include "types.h"
#include "interrupts.h"
#include <string.h>

namespace Interrupt
{
    static void DefaultHandler()
    {
        for(;;);
    }

    IDT::IDT()
        : m_idt{}
    {
        const uint32_t value = reinterpret_cast<uint32_t>(&DefaultHandler);
        for (uint16_t i = 0; i < SIZE; ++i)
        {
            m_irsTable[i] = reinterpret_cast<THandler>(value);
        }
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

                const uintptr_t address = reinterpret_cast<uintptr_t>(m_irsTable[i]);
                entry.offset_low = address & 0xFFFF;
                entry.offset_high = (address & 0xFFFF0000) >> 16;
            }
        }

        // Init IDT Pointer
        m_idtr.base = reinterpret_cast<uint32_t>(m_idt);
        m_idtr.limit = sizeof(m_idt) - 1;

        // Load IDT
        asm volatile("lidt %0" : : "m" (m_idtr));
    }

    bool IDT::AddDescriptor(uint32_t i, const idt_descriptor_t& descriptor)
    {
        if (i > SIZE)
        {
            return false;
        }              

        m_idt[i] = descriptor;
        return true;
    }

    const idt_descriptor_t& IDT::GetDescriptor(uint32_t i) const
    {
        return m_idt[i];
    }
};