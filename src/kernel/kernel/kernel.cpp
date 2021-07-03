#include "cli/cli.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/tty.h>

#ifdef __cplusplus
extern "C" {
#endif

void kernel_main()
{
	Cli cli;
	cli.Run();
}

#ifdef __cplusplus
}
#endif