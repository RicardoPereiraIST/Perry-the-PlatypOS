#include <stdio.h>
#include <kernel/tty.h>
#include "boot/utils/globals.h"
#include "boot/utils/helpers.h"
#include "boot/utils/multiboot.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int _kernel_start;
extern int _kernel_end;
extern int _kernel_file_size;

static inline void TestPrint()
{
	for (int i = 0; i < 50; ++i)
	{
		printf("Hello, %d kernel World!\n", i);
	}
}

static inline void TestAllocation()
{
	uint32_t* p = (uint32_t*)s_globals.PhysicalMemoryManager().AllocBlock();
	printf ("p allocated at 0x%x\n", p);

	uint32_t* p2 = (uint32_t*)s_globals.PhysicalMemoryManager().AllocBlocks(2);
	printf ("allocated 2 blocks for p2 at 0x%x\n", p2);

	s_globals.PhysicalMemoryManager().FreeBlock(p);
	p = (uint32_t*)s_globals.PhysicalMemoryManager().AllocBlock();
	printf ("Unallocated p to free block 1. p is reallocated to 0x%x\n", p);

	s_globals.PhysicalMemoryManager().FreeBlock(p);
	s_globals.PhysicalMemoryManager().FreeBlocks(p2, 2);
}

static inline void TestTicksForever()
{
	while(true)
	{
		printf("Tick: %d\n", s_globals.PIT().GetTickCount());
	}
}

void kernel_main(multiboot_info_t* pInfo, unsigned int magic)
{
	if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		s_globals.EarlyLogStorage().AddError("Wrong multiboot magic number.");
	}

	const uintptr_t start = (uintptr_t)&_kernel_start;
	const uintptr_t kernel_size = (uintptr_t)&_kernel_file_size;

	terminal_initialize();
	s_globals.StartTimer();

	if (!s_globals.PhysicalMemoryManager().Init(pInfo, start, kernel_size))
	{
		s_globals.EarlyLogStorage().AddError("Failed to initialize physical memory manager.");
	}

	// Can only test allocation before paging is enabled
	// TestAllocation();

	if (!s_globals.VirtualMemoryManager().Setup())
	{
		s_globals.EarlyLogStorage().AddError("Failed to initialize virtual memory manager.");
	}

	if (s_globals.EarlyLogStorage().HasErrors())
	{
		printf("Errors found: %d\n", s_globals.EarlyLogStorage().GetErrorCount());
	}

	// TestPrint();
	// TestTicksForever();
}

#ifdef __cplusplus
}
#endif