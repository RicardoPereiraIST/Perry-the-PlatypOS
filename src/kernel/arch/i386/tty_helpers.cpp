#include <kernel/tty_helpers.h>

/* static */ void* terminal_memset(void* bufptr, uint16_t value, size_t size)
{
	if (!bufptr)
	{
		return bufptr;
	}

	uint16_t* buf = (uint16_t*) bufptr;
	for (size_t i = 0; i < size; i++)
    {
		buf[i] = value;
    }
	return bufptr;
}