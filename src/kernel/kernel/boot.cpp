#include "boot/globals.h"
#include "boot/helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

void kernel_init()
{
    BootHelpers::DisableInterrupts();
    s_globals.ForceInitialization();
    BootHelpers::EnableInterrupts();
}

#ifdef __cplusplus
}
#endif