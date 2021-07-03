#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort()
{
	printf("Aborted...\n");
	while (1) { }
	__builtin_unreachable();
}
