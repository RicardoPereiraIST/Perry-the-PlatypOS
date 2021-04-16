#include <stdio.h>
#include <kernel/tty.h>

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
}

#ifdef __cplusplus
}
#endif