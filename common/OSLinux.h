#ifndef OS_H
#define OS_H

/********************  HEADERS  *********************/
#include <cstdlib>

/*********************  CLASS  **********************/
class OSLinux
{
	public:
		static void * mmap( void* addr, size_t size );
		static int munmap( void* addr, size_t size );
};

#endif //OS_H
