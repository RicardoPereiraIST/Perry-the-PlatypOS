#include <stdio.h>
#include <kernel/tty.h>
#include "boot/utils/globals.h"
#include "boot/utils/helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

void kernel_main()
{
	terminal_initialize();
	s_globals.StartTimer();

	for (int i = 0; i < 50; ++i)
	{
		printf("Hello, %d kernel World!\n", i);
	}

	if (s_globals.EarlyLogStorage().HasErrors())
	{
		printf("Errors found: %d\n", s_globals.EarlyLogStorage().GetErrorCount());
	}
}

#ifdef __cplusplus
}
#endif