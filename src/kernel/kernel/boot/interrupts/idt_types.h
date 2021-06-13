#pragma once

#include <stdint.h>
#include "registers.h"

namespace Interrupt
{
    struct idtr
    {
        uint16_t limit;
        uint32_t base;
    }__attribute__((packed));

    struct selector_t
    {
        uint16_t index : 13;
        uint8_t  ti  : 2;
        uint8_t  rpl : 1;
    }__attribute__((packed));

    struct flag_t
    {
        uint8_t gate_type    : 4;
        uint8_t s            : 1;
        uint8_t dpl          : 2;
        uint8_t p            : 1;
    }__attribute__((packed));

    struct idt_descriptor_t
    {
        uint16_t offset_low;
        selector_t selector;
        uint8_t zero;
        flag_t flags;
        uint16_t offset_high;
    }__attribute__((packed));

    static_assert(sizeof(selector_t) == 2, "IDT is 2 bytes long");
    static_assert(sizeof(flag_t) == 1, "IDT is 1 bytes long");
    static_assert(sizeof(idt_descriptor_t) == 8, "IDT is 8 bytes long");
    static_assert(sizeof(idtr) == 6, "IDTR is 6 bytes long");

    constexpr const uint16_t INTERRUPT_GATE_TYPE = 0xE;

    typedef void (*THandler)(Registers*);
}