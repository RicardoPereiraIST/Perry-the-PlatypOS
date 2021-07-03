#pragma once

#include "cstd.h"
#include "../gdt/gdt.h"
#include "../interrupts/idt.h"
#include "../interrupts/irq.h"
#include "../interrupts/isr.h"
#include "../pic/pic.h"
#include "../pit/pit.h"
#include "early_errors_storage.h"
#include "../physical_memory/PhysicalMemoryManager.h"
#include "../virtual_memory/VirtualMemoryManager.h"
#include "../../drivers/Keyboard/keyboard.h"

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
    ::BootHelpers::EarlyLogStorage& EarlyLogStorage();
    ::Memory::PhysicalMemoryManager& PhysicalMemoryManager();
    ::Memory::VirtualMemoryManager& VirtualMemoryManager();
    ::Devices::Keyboard& Keyboard();

private:
    ::GDT::GDT m_gdt;
    ::Interrupt::IDT m_idt;
    ::Interrupt::IRQ m_irq;
    ::Interrupt::ISR m_isr;
    ::PIC::PIC m_pic;
    ::PIT::PIT m_pit;
    ::BootHelpers::EarlyLogStorage m_errorStorage;
    ::Memory::PhysicalMemoryManager m_pmm;
    ::Memory::VirtualMemoryManager m_vmm;
    ::Devices::Keyboard m_keyboard;
};

extern Globals s_globals;