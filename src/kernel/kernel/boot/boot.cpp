#include "utils/globals.h"
#include "utils/helpers.h"

#include <stdio.h>
#include <kernel/tty.h>

#ifdef __cplusplus
extern "C" {
#endif

void kernel_init()
{
    BootHelpers::DisableInterrupts();

    terminal_initialize();
    s_globals.Setup();

    BootHelpers::EnableInterrupts();
}

#ifdef __cplusplus
}
#endif