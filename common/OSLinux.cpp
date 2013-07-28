/********************  HEADERS  *********************/
#include "OSLinux.h"
#include "Common.h"
//standard
#include <cassert>
#include <cstdio>
//for mmap/munmap
//TODO MOVE GNU_SOURCE in compile system, not put in source
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
int OSLinux::munmap ( void* addr, size_t size )
{
	//TODO check error
	int res = ::munmap(addr,size);
	if (res != 0)
		perror("Out of memory, failed to request memory to the OS via mmap.");
	return res;
}

/*******************  FUNCTION  *********************/
void* OSLinux::mremap ( void* addr, size_t old_size, size_t new_size, void* dest_addr )
{
	//errors
	assert(addr != NULL);
	assert(old_size % PAGE_SIZE == 0);
	assert(new_size % PAGE_SIZE == 0);
	
	void * res = ::mremap(addr,old_size,new_size,MREMAP_MAYMOVE);
	
	if (res == MAP_FAILED)
		perror("Out of memory, failed to remap memory via mremap.");
	
	return res;
}
