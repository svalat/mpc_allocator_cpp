#ifndef OS_H
#define OS_H

/********************  HEADERS  *********************/
#include <cstdlib>

/*********************  CLASS  **********************/
class OSLinux
{
	public:
		static void * mmap( void* addr, size_t size );
		static void munmap(void * addr, size_t size);
};

#endif //OS_H
