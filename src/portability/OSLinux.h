#ifndef OS_H
#define OS_H

/********************  HEADERS  *********************/
#include <cstdlib>

/********************  MACRO  ***********************/
#define OS_STDERR_FILENO 2

/*********************  CLASS  **********************/
class OSUnix
{
	public:
		static void * mmap( void* addr, size_t size );
		static int munmap( void* addr, size_t size );
		static size_t safeWrite(int fd,const char * value,size_t size);
};

/*********************  CLASS  **********************/
class OSLinux : public OSUnix
{
	public:
		static void * mremap( void * addr, size_t old_size, size_t new_size,void * dest_addr = NULL);
};

/*********************  TYPES  **********************/
typedef OSLinux OS;

#endif //OS_H
