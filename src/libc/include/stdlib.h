#pragma once

#include <sys/stddef.h>

#ifdef NDEBUG
# define assert(EX)
#else
# define assert(EX) (void)((EX) || (__assert (#EX, __FILE__, __LINE__),0))
#endif

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort();

void panic(const char* szMessage);

__attribute__((noreturn))
void __assert (const char *msg, const char *file, int line);

void __stack_chk_fail();

#ifdef __cplusplus
}
#endif