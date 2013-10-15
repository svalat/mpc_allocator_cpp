/********************  HEADERS  *********************/
#include <cstdio>
#include <cstring>
#include <cassert>
#include "EventDispatchReplay.h"

/********************  NAMESPACE  *******************/
using namespace MPCAllocator;

/********************  MACRO  ***********************/
//make it silent
#define trace_printf(...) do{} while(0)

/*******************  FUNCTION  *********************/
void EventDispatchReplay::checkIsZero(void * ptr,size_t size)
{
	for (size_t i = 0 ; i < size ; i++)
	{
		allocAssume(((char*)ptr)[i] == 0,"Required zero but get non initialized memory.");
	}
};

/*******************  FUNCTION  *********************/
void EventDispatchReplay::malloc ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t size, uint64_t result )
{
	//debug
	trace_printf("%p = malloc(%lu) //timestamp = %lu, thread = %d\n",(void*)result,size,timestamp,(int)threadId);

	//to alloc and print result
	void * ptr = alloc.malloc(size);
	trace_printf("          => %p , %lu\n",ptr,alloc.getInnerSize(ptr));
	
	//register to rememeber the current real addr for free/realloc.
	ptrs[result] = ptr;
	
	//register for checking
	checkerTrace.registerSegment((void*)result,size);
	checker.registerSegment(ptr,alloc.getInnerSize(ptr));
	
	//check usability
	memset(ptr,0,size);
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::free ( uint16_t threadId, uint64_t timestamp, uint16_t type, uint64_t ptr )
{
	//search real addr
	void * realPtr = ptrs[ptr];
	
	//debug
	trace_printf("free(%p) //timestamp = %lu, thread = %d, realptr = %p\n",(void*)ptr,timestamp,(int)threadId,realPtr);
	
	//mark as free
	checkerTrace.unregisterSegment((void*)ptr,1);
	checker.unregisterSegment(realPtr,alloc.getInnerSize(realPtr));
	
	//free it
	alloc.free(realPtr);
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::calloc ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t size, size_t nmemb, uint64_t result )
{
	//debug
	trace_printf("%p = calloc(%lu,%lu) //timestamp = %lu, thread = %d\n",(void*)result,nmemb,size,timestamp,(int)threadId);
	
	//to calloc
	void * ptr = alloc.calloc(nmemb,size);
	
	//print and reg real ptr
	trace_printf("          => %p , %lu\n",ptr,alloc.getInnerSize(ptr));
	ptrs[result] = ptr;
	
	//register for checking
	checkerTrace.registerSegment((void*)result,nmemb*size);
	checker.registerSegment(ptr,alloc.getInnerSize(ptr));
	
	//check usage and if zero
	checkIsZero(ptr,nmemb*size);
	memset(ptr,0,size);
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::realloc ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t oldPtr, size_t newSize, uint64_t result )
{
	//search real ptr
	void * inPtr = ptrs[oldPtr];
	
	//print debug
	trace_printf("%p = relloc(%p,%lu) //timestamp = %lu, thread = %d, realptr = %p\n",(void*)result,(void*)oldPtr,newSize,timestamp,(int)threadId,ptr);
	
	//update registration
	checkerTrace.unregisterSegment((void*)oldPtr,1);
	checkerTrace.registerSegment((void*)result,newSize);
	checker.unregisterSegment(inPtr,alloc.getInnerSize(inPtr));
	
	//do realloc
	void * outPtr = alloc.realloc(inPtr,newSize);
	trace_printf("          => %p , %lu\n",ptr,alloc.getInnerSize(ptr));
	ptrs[result] = outPtr;
	
	//update registration
	checker.registerSegment(outPtr,alloc.getInnerSize(outPtr));
	
	//TODO check value copy
	
	//check usability
	memset(outPtr,0,newSize);
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::mallocAlign ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t size, size_t align, uint64_t result )
{
	//print debug
	trace_printf("%p = memalign(%lu,%lu) //timestamp = %lu, thread = %d\n",(void*)result,align,size,timestamp,(int)threadId);
	
	//do alloc
	void * ptr = alloc.memalign(align,size);
	
	//debug and remember ptr
	trace_printf("          => %p , %lu\n",ptr,alloc.getInnerSize(ptr));
	ptrs[result] = ptr;
	
	//checking
	checkerTrace.registerSegment((void*)result,size);
	checker.registerSegment(ptr,alloc.getInnerSize(ptr));
	
	//check align
	assert(result % align == 0);
	assert((Addr)ptr % align == 0);
	
	//check usage
	memset(ptr,0,size);
}
