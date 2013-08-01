/********************  HEADERS  *********************/
#include "OS.h"
#include <Common.h>
#include <Debug.h>
//standard
#include <cassert>
#include <cstdio>
//for mmap/munmap
//for mremap require -D_GNU_SOURCE on compiler
#include <sys/mman.h>
//for write
#include <unistd.h>

/*******************  FUNCTION  *********************/
void* OSUnix::mmap ( void* addr, size_t size )
{
	void * res = NULL;

	if (addr == NULL)
		res = ::mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE,-1,0);
	else
		res = ::mmap(addr,size,PROT_READ|PROT_WRITE,MAP_ANON|MAP_PRIVATE|MAP_FIXED,-1,0);
	
	//check error
	allocAssumePerror(res != MAP_FAILED,"Out of memory, failed to request memory to the OS via mmap.");
	
	//ok return
	return res;
}

/*******************  FUNCTION  *********************/
int OSUnix::munmap ( void* addr, size_t size )
{
	//TODO check error
	int res = ::munmap(addr,size);
	allocAssumePerror(res == 0,"Failed to return memory to the OS via munmap.");
	return res;
}

/*******************  FUNCTION  *********************/
size_t OSUnix::safeWrite ( int fd, const char* value, size_t size )
{
	return write(fd,value,size);
	//TODO make check, see MPC version of this
}

/*******************  FUNCTION  *********************/
void* OSLinux::mremap ( void* addr, size_t old_size, size_t new_size, void* dest_addr )
{
	//errors
	assert(addr != NULL);
	assert(old_size % PAGE_SIZE == 0);
	assert(new_size % PAGE_SIZE == 0);
	
	void * res = ::mremap(addr,old_size,new_size,MREMAP_MAYMOVE);
	
	allocAssumePerror(res != MAP_FAILED,"Failed to remap memory via mremap.");
	
	return res;
}
