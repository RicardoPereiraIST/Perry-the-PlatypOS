#pragma once

#include <stdint.h>

namespace Interrupt
{
    struct idtr
    {
        uint16_t base;
        uint32_t limit;
    };

    struct selector_t
    {
        uint8_t  rpl : 1;
        uint8_t  ti  : 2;
        uint16_t index : 13;
    }__attribute__((packed));

    struct flag_t
    {
        uint8_t gate_type    : 4;
        uint8_t s            : 1;
        uint8_t dpl          : 2;
        uint8_t p            : 1;
    };

    struct idt_descriptor_t
    {
        uint16_t offset_low  : 16;
        selector_t selector;
        uint8_t zero         : 8;
        flag_t flags;
        uint16_t offset_high : 16;
    }__attribute__((packed));

    static_assert(sizeof(idt_descriptor_t) == 8, "IDT is 8 bytes long");

    constexpr const uint16_t INTERRUPT_GATE_TYPE = 0xE;

    class IDT
    {
        typedef void (*THandler)();
        static constexpr uint16_t SIZE = 256;
    public:
        IDT();
        void Setup();

    private:
        idtr m_idtr;
        idt_descriptor_t m_idt[SIZE];
        THandler m_irsTable[SIZE];
    };
}