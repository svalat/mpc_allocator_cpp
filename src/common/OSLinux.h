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
		static void * mremap( void * addr, size_t old_size, size_t new_size,void * dest_addr = NULL);
};

#endif //OS_H
