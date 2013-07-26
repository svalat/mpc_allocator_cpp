/********************  HEADERS  *********************/
#include "OSLinux.h"
//standard
#include <cassert>
#include <cstdio>
//for mmap/munmap
#include <sys/mman.h>

/*******************  FUNCTION  *********************/
void* OSLinux::mmap ( void* addr, size_t size )
{
	void * res = NULL;
	if (addr == NULL)
		res = ::mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE,-1,0);
	else
		res = ::mmap(addr,size,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE|MAP_FIXED,-1,0);
	if (res == MAP_FAILED)
		perror("Out of memory, failed to request memory to the OS via mmap.");
	return res;
}

/*******************  FUNCTION  *********************/
void OSLinux::munmap ( void* addr, size_t size )
{
	//TODO check error
	::munmap(addr,size);
}
