/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include "EventDispatchPrint.h"

/*******************  FUNCTION  *********************/
void EventDispatchPrint::calloc ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t size, size_t nmemb, uint64_t result )
{
	printf("%p = calloc(%lu,%lu) //timestamp = %lu, thread = %d\n",(void*)result,nmemb,size,timestamp,(int)threadId);
}

/*******************  FUNCTION  *********************/
void EventDispatchPrint::free ( uint16_t threadId, uint64_t timestamp, uint16_t type, uint64_t ptr )
{
	printf("free(%p) //timestamp = %lu, thread = %d\n",(void*)ptr,timestamp,threadId);
}

/*******************  FUNCTION  *********************/
void EventDispatchPrint::malloc ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t size, uint64_t result )
{
	printf("%p = malloc(%lu) //timestamp = %lu, thread = %d\n",(void*)result,size,timestamp,(int)threadId);
}

/*******************  FUNCTION  *********************/
void EventDispatchPrint::realloc ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t oldPtr, size_t newSize, uint64_t result )
{
	printf("%p = relloc(%p,%lu) //timestamp = %lu, thread = %d\n",(void*)result,(void*)oldPtr,newSize,timestamp,(int)threadId);
}

/*******************  FUNCTION  *********************/
void EventDispatchPrint::mallocAlign ( uint16_t threadId, uint64_t timestamp, uint16_t type, size_t size, size_t align, uint64_t result )
{
	printf("%p = memalign(%lu,%lu) //timestamp = %lu, thread = %d\n",(void*)result,align,size,timestamp,(int)threadId);
}
