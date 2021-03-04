#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* terminal_memset(void* bufptr, uint16_t value, size_t size);

#ifdef __cplusplus
}
#endif