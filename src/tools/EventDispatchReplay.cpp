/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

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
// #define trace_printf printf
// #define optional_memset(...) do{} while(0)
#define optional_memset memset

/*******************  FUNCTION  *********************/
EventDispatchReplay::EventDispatchReplay ( bool checkTrace )
{
	this->checkTrace = checkTrace;
}

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
	alloc.hardChecking();
	void * ptr = alloc.malloc(size);
	alloc.hardChecking();
	trace_printf("          => %p , %lu\n",ptr,alloc.getInnerSize(ptr));
	
	//register to rememeber the current real addr for free/realloc.
	ptrs[result] = ptr;
	
	//register for checking
	if (checkTrace)
		checkerTrace.registerSegment((void*)result,size);
	checker.registerSegment(ptr,alloc.getInnerSize(ptr));
	
	//check usability
	optional_memset(ptr,0,size);
	alloc.hardChecking();
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::free ( uint16_t threadId, uint64_t timestamp, uint16_t type, uint64_t ptr )
{
	//search real addr
	void * realPtr = getRealPtr(ptr);
	
	//debug
	trace_printf("free(%p) //timestamp = %lu, thread = %d, realptr = %p\n",(void*)ptr,timestamp,(int)threadId,realPtr);
	
	//mark as free
	if (realPtr != NULL)
	{
		if (checkTrace)
			checkerTrace.unregisterSegment((void*)ptr,1);
		checker.unregisterSegment(realPtr,alloc.getInnerSize(realPtr));
	}
	
	//free it
	alloc.hardChecking();
	alloc.free(realPtr);
	alloc.hardChecking();
	
	//mark as free
	if (realPtr != NULL)
		ptrs.erase(ptr);
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::calloc ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t size, size_t nmemb, uint64_t result )
{
	//debug
	trace_printf("%p = calloc(%lu,%lu) //timestamp = %lu, thread = %d\n",(void*)result,nmemb,size,timestamp,(int)threadId);
	
	//to calloc
	alloc.hardChecking();
	void * ptr = alloc.calloc(nmemb,size);
	alloc.hardChecking();
	
	//print and reg real ptr
	trace_printf("          => %p , %lu\n",ptr,alloc.getInnerSize(ptr));
	ptrs[result] = ptr;
	
	//register for checking
	if (checkTrace)
		checkerTrace.registerSegment((void*)result,nmemb*size);
	checker.registerSegment(ptr,alloc.getInnerSize(ptr));
	
	//check usage and if zero
	checkIsZero(ptr,nmemb*size);
	optional_memset(ptr,0,size);
	alloc.hardChecking();
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::realloc ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t oldPtr, size_t newSize, uint64_t result )
{
	//search real ptr
	void * inPtr = getRealPtr(oldPtr);
	
	//print debug
	trace_printf("%p = relloc(%p,%lu) //timestamp = %lu, thread = %d, realptr = %p\n",(void*)result,(void*)oldPtr,newSize,timestamp,(int)threadId,inPtr);
	
	//update registration
	if (inPtr != NULL)
	{
		if (checkTrace)
			checkerTrace.unregisterSegment((void*)oldPtr,1);
		checker.unregisterSegment(inPtr,alloc.getInnerSize(inPtr));
		ptrs.erase(oldPtr);
	}
	
	if (checkTrace)
		checkerTrace.registerSegment((void*)result,newSize);
	
	//put data in buffer
	size_t s = 0;
	if (inPtr != NULL)
	{
		alloc.hardChecking();
		s = alloc.getInnerSize(inPtr);
		for (size_t i = 0 ; i < s ; i++)
			((char*)inPtr)[i] = (char)i;
	}
	
	//do realloc
	alloc.hardChecking();
	void * outPtr = alloc.realloc(inPtr,newSize);
	alloc.hardChecking();
	trace_printf("          => %p , %lu\n",outPtr,alloc.getInnerSize(outPtr));
	ptrs[result] = outPtr;
	
	//update registration
	checker.registerSegment(outPtr,alloc.getInnerSize(outPtr));
	
	//check if content was OK
	if (outPtr != NULL)
	{
		if (newSize < s)
			s = newSize;
		for (size_t i = 0 ; i < s ; i++)
		{
			if (((char*)outPtr)[i] != (char)i)
			{
				allocWarning("Fail to keep content on realloc.");
				break;
			}
		}	
	}
	
	//check usability
	optional_memset(outPtr,0,newSize);
	alloc.hardChecking();
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::mallocAlign ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t size, size_t align, uint64_t result )
{
	//print debug
	trace_printf("%p = memalign(%lu,%lu) //timestamp = %lu, thread = %d\n",(void*)result,align,size,timestamp,(int)threadId);
	
	//do alloc
	alloc.hardChecking();
	void * ptr = alloc.memalign(align,size);
	alloc.hardChecking();
	
	//debug and remember ptr
	trace_printf("          => %p , %lu\n",ptr,alloc.getInnerSize(ptr));
	ptrs[result] = ptr;
	
	//checking
	if (checkTrace)
		checkerTrace.registerSegment((void*)result,size);
	checker.registerSegment(ptr,alloc.getInnerSize(ptr));
	
	//check align
	assert(result % align == 0);
	assert((Addr)ptr % align == 0);
	
	//check usage
	optional_memset(ptr,0,size);
	alloc.hardChecking();
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::cleanupAllMemory ( void )
{
	alloc.cleanupRegisterdMemory();
}

/*******************  FUNCTION  *********************/
EventDispatchReplay::~EventDispatchReplay ( void )
{
	cleanupAllMemory();
}

/*******************  FUNCTION  *********************/
void EventDispatchReplay::resetTLS ( void )
{
	alloc.resetTLSForTest();
}

/*******************  FUNCTION  *********************/
void* EventDispatchReplay::getRealPtr ( uint64_t ref )
{
	std::map<uint64_t,void*>::const_iterator it = ptrs.find(ref);
	if (it == ptrs.end())
		return NULL;
	else
		return it->second;
}
