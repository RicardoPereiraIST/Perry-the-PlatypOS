#include "isr.h"
#include "../utils/globals.h"

extern "C" {
    void _isr0(Registers*);
    void _isr1(Registers*);
    void _isr2(Registers*);
    void _isr3(Registers*);
    void _isr4(Registers*);
    void _isr5(Registers*);
    void _isr6(Registers*);
    void _isr7(Registers*);
    void _isr8(Registers*);
    void _isr9(Registers*);
    void _isr10(Registers*);
    void _isr11(Registers*);
    void _isr12(Registers*);
    void _isr13(Registers*);
    void _isr14(Registers*);
    void _isr15(Registers*);
    void _isr16(Registers*);
    void _isr17(Registers*);
    void _isr18(Registers*);
    void _isr19(Registers*);
    void _isr20(Registers*);
    void _isr21(Registers*);
    void _isr22(Registers*);
    void _isr23(Registers*);
    void _isr24(Registers*);
    void _isr25(Registers*);
    void _isr26(Registers*);
    void _isr27(Registers*);
    void _isr28(Registers*);
    void _isr29(Registers*);
    void _isr30(Registers*);
    void _isr31(Registers*);
    void _isr32(Registers*);
    void _isr33(Registers*);
    void _isr34(Registers*);
    void _isr35(Registers*);
    void _isr36(Registers*);
    void _isr37(Registers*);
    void _isr38(Registers*);
    void _isr39(Registers*);
    void _isr40(Registers*);
    void _isr41(Registers*);
    void _isr42(Registers*);
    void _isr43(Registers*);
    void _isr44(Registers*);
    void _isr45(Registers*);
    void _isr46(Registers*);
    void _isr47(Registers*);
}

static void (*stubs[Interrupt::NUM_ISRS])(Registers*) = {
    _isr0,
    _isr1,
    _isr2,
    _isr3,
    _isr4,
    _isr5,
    _isr6,
    _isr7,
    _isr8,
    _isr9,
    _isr10,
    _isr11,
    _isr12,
    _isr13,
    _isr14,
    _isr15,
    _isr16,
    _isr17,
    _isr18,
    _isr19,
    _isr20,
    _isr21,
    _isr22,
    _isr23,
    _isr24,
    _isr25,
    _isr26,
    _isr27,
    _isr28,
    _isr29,
    _isr30,
    _isr31,
    _isr32,
    _isr33,
    _isr34,
    _isr35,
    _isr36,
    _isr37,
    _isr38,
    _isr39,
    _isr40,
    _isr41,
    _isr42,
    _isr43,
    _isr44,
    _isr45,
    _isr46,
    _isr47,
};

static constexpr const char* const exceptions[32] = {
    "Divide by zero",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "OOB",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unrecognized interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED",
    "RESERVED"
};

extern "C" {
    void isr_handler(Registers *regs)
    {
        Interrupt::ISR::Call(regs);
    }
}

static void exception_handler(Registers *regs)
{
    // panic(exceptions[regs->int_no]);
    for(;;);
}

namespace Interrupt
{
    THandler ISR::s_handlers[]{};
    ISR::ISRS ISR::s_isrs[]{};

    void ISR::Setup()
    {
        selector_t selector{};
        flag_t flags{};

        selector.index = GDT::CODE_SELECTOR;

        flags.p = 1;
        flags.gate_type = INTERRUPT_GATE_TYPE;

        for (uint8_t i = 0; i < NUM_ISRS; i++)
        {
            s_isrs[i].index = i;
            s_isrs[i].stub = stubs[i];
            s_globals.IDT().AddDescriptor(s_isrs[i].index, s_isrs[i].stub, selector, flags);
        }

        for (uint8_t i = 0; i < 32; i++)
        {
            Add(i, exception_handler);
        }
    }

    void ISR::Add(uint8_t i, THandler handler)
    {
        s_handlers[i] = handler;
    }

    void ISR::Call(Registers* regs)
    {
        if (Interrupt::ISR::s_handlers[regs->int_no])
        {
            Interrupt::ISR::s_handlers[regs->int_no](regs);
        }
    }
}