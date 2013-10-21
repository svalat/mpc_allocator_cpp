/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef ALLOC_EVENT_DISPATCH_H
#define ALLOC_EVENT_DISPATCH_H

/********************  HEADERS  *********************/
#include "AllocTraceStruct.h"
#include "TraceReader.h"

/*********************  CLASS  **********************/
class EventDispatch
{
	public:
		virtual void run(TraceReader & reader);
		virtual void run(MPCAllocator::TraceEntryComplete & entry);
		virtual ~EventDispatch(void) {};
	protected:
		virtual void malloc(uint16_t threadId,uint64_t timestamp,uint16_t type,size_t size,uint64_t result) = 0;
		virtual void mallocAlign(uint16_t threadId,uint64_t timestamp,uint16_t type,size_t size,size_t align,uint64_t result) = 0;
		virtual void calloc(uint16_t threadId,uint64_t timestamp,uint16_t type,size_t size,size_t nmemb,uint64_t result) = 0;
		virtual void free(uint16_t threadId,uint64_t timestamp,uint16_t type,uint64_t ptr) = 0;
		virtual void realloc(uint16_t threadId,uint64_t timestamp,uint16_t type,size_t old_ptr,size_t new_size,uint64_t result) = 0;
};

#endif //ALLOC_EVENT_DISPATCH_H
