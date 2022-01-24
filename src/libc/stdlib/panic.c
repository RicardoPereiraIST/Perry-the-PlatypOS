#include <stdio.h>
#include <stdlib.h>

void panic(const char* szMessage)
{
    printf("Panic: %s\n", szMessage);
	abort();
}