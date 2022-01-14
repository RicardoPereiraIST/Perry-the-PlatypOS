#pragma once

#include <stddef.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsized-deallocation"

// purecall function handler
extern "C" inline void __cxa_pure_virtual()
{
    // Do nothing or print an error message.
}

// global new and delete operators
void* operator new (size_t size) throw();
void* operator new[] (size_t size) throw();
void operator delete (void* p) throw();
void operator delete[] (void* p) throw();
void* operator new(size_t size, void* p) throw();
void* operator new[](size_t size, void* p) throw();

namespace __cxxabiv1 
{
	/* guard variables */
 
	/* The ABI requires a 64-bit type.  */
	__extension__ typedef int __guard __attribute__((mode(__DI__)));
 
	extern "C" inline int __cxa_guard_acquire (__guard *);
	extern "C" void __cxa_guard_release (__guard *);
	extern "C" void __cxa_guard_abort (__guard *);
 
	extern "C" inline int __cxa_guard_acquire (__guard *g) 
	{
		return !*(char *)(g);
	}
 
	extern "C" inline void __cxa_guard_release (__guard *g)
	{
		*(char *)g = 1;
	}
 
	extern "C" inline void __cxa_guard_abort (__guard *)
	{
 
	}
}

#pragma GCC diagnostic pop