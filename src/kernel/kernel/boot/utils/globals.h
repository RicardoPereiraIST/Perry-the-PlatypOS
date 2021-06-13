#pragma once

#include "cstd.h"
#include "../gdt/gdt.h"
#include "../interrupts/idt.h"
#include "../interrupts/irq.h"
#include "../interrupts/isr.h"
#include "../pic/pic.h"
#include "../pit/pit.h"

class Globals
{
public:
    Globals();
    ~Globals() = default;

    void Setup();
    void StartTimer();

    ::GDT::GDT& GDT();
    ::Interrupt::IDT& IDT();
    ::Interrupt::IRQ& IRQ();
    ::Interrupt::ISR& ISR();
    ::PIC::PIC& PIC();
    ::PIT::PIT& PIT();

private:
    ::GDT::GDT m_gdt;
    ::Interrupt::IDT m_idt;
    ::Interrupt::IRQ m_irq;
    ::Interrupt::ISR m_isr;
    ::PIC::PIC m_pic;
    ::PIT::PIT m_pit;
};

extern Globals s_globals;