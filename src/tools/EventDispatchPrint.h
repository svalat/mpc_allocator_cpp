/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef ALLOC_EVENT_DISPATCH_PRINT_H
#define ALLOC_EVENT_DISPATCH_PRINT_H

/********************  HEADERS  *********************/
#include "EventDispatch.h"

/*********************  CLASS  **********************/
class EventDispatchPrint : public EventDispatch
{
	protected:
		virtual void malloc(uint16_t threadId,uint64_t timestamp,uint16_t type,size_t size,uint64_t result);
		virtual void mallocAlign(uint16_t threadId,uint64_t timestamp,uint16_t type,size_t size,size_t align,uint64_t result);
		virtual void calloc(uint16_t threadId,uint64_t timestamp,uint16_t type,size_t size,size_t nmemb,uint64_t result);
		virtual void free(uint16_t threadId,uint64_t timestamp,uint16_t type,uint64_t ptr);
		virtual void realloc(uint16_t threadId,uint64_t timestamp,uint16_t type,size_t old_ptr,size_t new_size,uint64_t result);
};

#endif //ALLOC_EVENT_DISPATCH_PRINT_H
