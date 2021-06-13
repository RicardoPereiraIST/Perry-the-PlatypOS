#include "globals.h"
#include "../pit/pit_types.h"

Globals s_globals;

void Globals::Setup()
{
    m_gdt.Setup();
    m_idt.Setup();
    m_pic.Setup(0x20,0x28);
    m_isr.Setup();
    m_irq.Setup();
}

void Globals::StartTimer()
{
    m_pit.StartCounter(363, PIT::OCW_COUNTER_0, PIT::OCW_MODE_SQUAREWAVEGEN);
}

Globals::Globals()
{
}

::GDT::GDT& Globals::GDT()
{
    return m_gdt;
}

::Interrupt::IDT& Globals::IDT()
{
    return m_idt;
}

::Interrupt::IRQ& Globals::IRQ()
{
    return m_irq;
}

::Interrupt::ISR& Globals::ISR()
{
    return m_isr;
}

::PIC::PIC& Globals::PIC()
{
    return m_pic;
}

::PIT::PIT& Globals::PIT()
{
    return m_pit;
}