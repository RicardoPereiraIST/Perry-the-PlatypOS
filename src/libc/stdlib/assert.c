#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void __assert(const char *msg, const char *file, int line)
{
	printf("Assert: %s on file %s, line %d", msg, file, line);
	abort();
}
