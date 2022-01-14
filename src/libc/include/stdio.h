#pragma once

#include <sys/stddef.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);
int atoi(const char* str);

#ifdef __cplusplus
}
#endif