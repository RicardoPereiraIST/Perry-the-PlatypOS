#include <sys/cstd.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsized-deallocation"

void* operator new (size_t size) throw() { return nullptr; }
void* operator new[] (size_t size) throw() { return nullptr; }
void operator delete (void* p) throw() {}
void operator delete[] (void* p) throw() {}
void* operator new(size_t size, void* p) throw() { return p; }
void* operator new[](size_t size, void* p) throw() { return p; }

#pragma GCC diagnostic pop