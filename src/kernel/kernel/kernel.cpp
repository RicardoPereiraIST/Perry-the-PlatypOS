#include <stdio.h>
#include <kernel/tty.h>
#include "boot/globals.h"

#ifdef __cplusplus
extern "C" {
#endif

void kernel_main()
{
	terminal_initialize();
	for (int i = 0; i < 50; ++i)
	{
		printf("Hello, %d kernel World!\n", i);
	}

	while(true)
	{
		printf("%d\n", s_globals.PIT().GetTickCount());
	}
}

#ifdef __cplusplus
}
#endif