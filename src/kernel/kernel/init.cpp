// Adapted from https://github.com/wichtounet/thor-os/

#include "boot/gdt/gdt_initializer.h"
#include "boot/gdt/interrupts.h"

GDT::CGDT gdt;
Interrupt::IDT idt;

#ifdef __cplusplus
extern "C" {
#endif

void kernel_init()
{
    gdt.Setup();
    idt.Setup();
}

#ifdef __cplusplus
}
#endif