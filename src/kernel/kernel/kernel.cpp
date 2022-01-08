#include "cli/cli.h"
// #include <sys/cstd.h>
// #include <sys/icxxabi.h>
#include "boot/utils/globals.h"
#include "boot/utils/global_helpers.h"

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