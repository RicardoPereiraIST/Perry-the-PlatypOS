#include "cli/cli.h"
#include "boot/utils/globals.h"
#include "boot/utils/global_helpers.h"

#ifdef __cplusplus
extern "C" {
#endif


void kernel_main()
{
	printf("Welcome to PerryOS!\n");
	Cli cli;
	cli.Run();	
}

#ifdef __cplusplus
}
#endif