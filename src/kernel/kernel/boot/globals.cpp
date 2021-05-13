#include "globals.h"

Globals s_globals;

void Globals::ForceInitialization()
{
    m_gdt.Setup();
    m_idt.Setup();
    m_pic.Setup(0x20,0x28);
    m_pit.Setup();
    m_pit.StartCounter(100, I86_PIT_OCW_COUNTER_0, I86_PIT_OCW_MODE_SQUAREWAVEGEN);
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

::PIC& Globals::PIC()
{
    return m_pic;
}

::PIT& Globals::PIT()
{
    return m_pit;
}