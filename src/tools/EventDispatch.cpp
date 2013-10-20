/********************  HEADERS  *********************/
#include <cstdio>
#include "EventDispatch.h"

/********************  NAMESPACE  *******************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
void EventDispatch::run ( TraceReader& reader )
{
	TraceEntryComplete entry;
	
	while (reader.readNext(entry))
	{
		this->run(entry);
	}
}

/*******************  FUNCTION  *********************/
void EventDispatch::run ( MPCAllocator::TraceEntryComplete& entry )
{
	//vars
	uint16_t type = entry.call.type;
	uint64_t threadId = entry.call.threadId;
	uint64_t timestamp = entry.call.timestamp;
	TraceMallocArgs & mallocArgs = entry.call.infos.malloc;
	TraceFreeArgs & freeArgs = entry.call.infos.free;
	TraceCallocArgs & callocArgs = entry.call.infos.calloc;
	TraceReallocArgs & reallocArgs = entry.call.infos.realloc;
	TraceAlignAllocArgs & alignArgs = entry.call.infos.memalign;
	
	switch(type)
	{
		case TRACE_MALLOC:
			this->malloc(threadId,timestamp,type,mallocArgs.size,entry.result);
			break;
		case TRACE_FREE:
			this->free(threadId,timestamp,type,freeArgs.ptr);
			break;
		case TRACE_CALLOC:
			this->calloc(threadId,timestamp,type,callocArgs.size,callocArgs.nmemb,entry.result);
			break;
		case TRACE_REALLOC:
			this->realloc(threadId,timestamp,type,reallocArgs.oldPtr,reallocArgs.newSize,entry.result);
			break; 
		case TRACE_MEMALIGN:
		case TRACE_ALIGN_ALLOC:
		case TRACE_POSIX_MEMALIGN:
			this->mallocAlign(threadId,timestamp,type,alignArgs.size,alignArgs.align,entry.result);
			break;
		case TRACE_PVALLOC:
		case TRACE_VALLOC:
			fprintf(stderr,"pvalloc/valloc not supported\n");
			abort();
			break;
		default:
			fprintf(stderr,"Invalid event type %d\n",type);
			abort();
	}
}
