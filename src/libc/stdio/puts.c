#include <stdio.h>

int puts(const char* string)
{
	if (!string)
	{
		return EOF;
	}

	return printf("%s\n", string);
}
