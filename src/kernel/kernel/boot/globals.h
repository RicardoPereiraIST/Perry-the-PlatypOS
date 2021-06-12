#pragma once

#include "cstd.h"
#include "tables/gdt.h"
#include "tables/idt.h"
#include "tables/irq.h"
#include "tables/isr.h"
#include "tables/pic.h"
#include "tables/pit.h"

class Globals
{
public:
    Globals();
    ~Globals() = default;

    void ForceInitialization();

    ::GDT::GDT& GDT();
    ::Interrupt::IDT& IDT();
    ::Interrupt::IRQ& IRQ();
    ::Interrupt::ISR& ISR();
    ::PIC& PIC();
    ::PIT& PIT();

private:
    ::GDT::GDT m_gdt;
    ::Interrupt::IDT m_idt;
    ::Interrupt::IRQ m_irq;
    ::Interrupt::ISR m_isr;
    ::PIC m_pic;
    ::PIT m_pit;
};

extern Globals s_globals;