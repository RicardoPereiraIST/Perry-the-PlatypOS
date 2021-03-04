#include <string.h>

void* memmove(void* dstptr, const void* srcptr, size_t size)
{
	if (!(dstptr || srcptr))
	{
		return dstptr;
	}

	unsigned char* const dst = (unsigned char* const) dstptr;
	const unsigned char* const src = (const unsigned char* const) srcptr;
	if (dst < src)
	{
		return memcpy(dstptr, srcptr, size);
	}
	else
	{
		for (size_t i = size; i != 0; i--)
		{
			dst[i-1] = src[i-1];
		}
	}
	return dstptr;
}
