#pragma once

#include <sys/stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
size_t strlen(const char*);
char* itoa(int value, char* result, int base);
char* strcpy(char* s1, const char* s2);
char* strchr(char* str, int character);

#ifdef __cplusplus
}
#endif