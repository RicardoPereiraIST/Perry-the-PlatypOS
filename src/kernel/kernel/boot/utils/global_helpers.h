#pragma once

#include <stdint.h>
#include "globals.h"

static inline void sleep (uint32_t ms)
{
    const uint32_t ticks = ms + s_globals.PIT().GetTickCount();
    while (ticks > s_globals.PIT().GetTickCount());
}