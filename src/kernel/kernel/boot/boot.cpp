#include "utils/globals.h"
#include "utils/helpers.h"

#ifdef __cplusplus
extern "C" {
#endif

void kernel_init()
{
    BootHelpers::DisableInterrupts();
    s_globals.Setup();
    BootHelpers::EnableInterrupts();
}

#ifdef __cplusplus
}
#endif