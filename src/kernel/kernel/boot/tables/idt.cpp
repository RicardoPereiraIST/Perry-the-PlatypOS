#include "types.h"
#include "idt.h"
#include <string.h>

namespace Interrupt
{
    IDT::IDT()
        : m_idt{}
    {}


    void IDT::Setup()
    {
        // Init IDT Pointer
        m_idtr.base = reinterpret_cast<uint32_t>(m_idt);
        m_idtr.limit = sizeof(m_idt) - 1;

        // Load IDT
        asm volatile("lidt %0" : : "m" (m_idtr));
    }

    bool IDT::AddDescriptor(uint8_t i, THandler handler, selector_t selector, flag_t flags)
    {
        if (i > SIZE)
        {
            return false;
        }

        idt_descriptor_t descriptor{};
        descriptor.offset_low = (reinterpret_cast<uintptr_t>(handler)) & 0xFFFF;
        descriptor.offset_high = (reinterpret_cast<uintptr_t>(handler) >> 16) & 0xFFFF;
        descriptor.selector = selector;
        descriptor.flags = flags;

        m_idt[i] = descriptor;

        return true;
    }
};