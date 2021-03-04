#pragma once

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort();

__attribute__((noreturn))
void __stack_chk_fail();

#ifdef __cplusplus
}
#endif