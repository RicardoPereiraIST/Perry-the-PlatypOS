#pragma once

#include "cstd.h"
#include "tables/gdt.h"
#include "tables/interrupts.h"
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
    ::PIC& PIC();
    ::PIT& PIT();

private:
    ::GDT::GDT m_gdt;
    ::Interrupt::IDT m_idt;
    ::PIC m_pic;
    ::PIT m_pit;
};

extern Globals s_globals;