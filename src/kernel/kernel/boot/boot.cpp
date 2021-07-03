#include "utils/globals.h"
#include "utils/helpers.h"
#include "utils/multiboot.h"

#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>

#ifdef __cplusplus
extern "C" {
#endif

static void SetupMemory(multiboot_info_t* pInfo)
{
    extern int _kernel_start;		// start of linked.ld
    extern int _kernel_end;			// end of linked.ld
    extern int _kernel_file_size;	// after data block -> linker.ld

	(void) _kernel_end;				// using _kernel_file_size instead, as bss is not needed to account for

    const uintptr_t start = (uintptr_t)&_kernel_start;
	const uintptr_t kernel_size = (uintptr_t)&_kernel_file_size;

	if (!s_globals.PhysicalMemoryManager().Init(pInfo, start, kernel_size))
	{
		panic("Failed to initialize physical memory manager.\n");
	}

	if (!s_globals.VirtualMemoryManager().Setup())
	{
		panic("Failed to initialize virtual memory manager.\n");
	}
}

void kernel_init(multiboot_info_t* pInfo, unsigned int magic)
{
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		panic("Wrong multiboot magic number.\n");
	}

    BootHelpers::DisableInterrupts();

    terminal_initialize();
    s_globals.Setup();
	s_globals.StartTimer();
    SetupMemory(pInfo);
	s_globals.Keyboard().Setup();

    BootHelpers::EnableInterrupts();
}

#ifdef __cplusplus
}
#endif