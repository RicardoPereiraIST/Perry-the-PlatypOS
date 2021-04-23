// Adapted from https://github.com/wichtounet/thor-os/

#include "boot/tables/gdt.h"
#include "boot/tables/interrupts.h"
#include "boot/a20/a20_enabler.h"

GDT::GDT gdt;
Interrupt::IDT idt;

// Remove Unused Function warning, since this is not called as it breaks qemu
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static bool TryEnableA20()
{
    A20Enabler a20Enabler;
    return a20Enabler.TryEnableA20();
}
#pragma GCC diagnostic pop


static void EnableInterrupts(bool enable)
{
    if (enable)
    {
        asm volatile("sli;");
    }
    else
    {
        asm volatile("cli;");
    }
}


#ifdef __cplusplus
extern "C" {
#endif

void kernel_init()
{
    EnableInterrupts(false);
    
    gdt.Setup();
    idt.Setup();

    // Does not work in qemu
    // TryEnableA20();
}

#ifdef __cplusplus
}
#endif