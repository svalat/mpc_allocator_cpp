/********************  HEADERS  *********************/
#include <cstdio>
#include <cassert>
#include <PosixAllocator.h>
#include <map>
#include "TraceReader.h"
#include "CollisionChecker.h"

/**********************  USING  *********************/
using namespace std;
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	assert(argc == 2);
	TraceReader reader(argv[1]);
	TraceEntryComplete entry;
	PosixAllocator alloc;
	std::map<uint64_t,void*> ptrs;
	CollisionChecker checker;
	CollisionChecker checkerTrace;
	void * ptr;
	
	while (reader.readNext(entry))
	{
		switch(entry.call.type)
		{
			case TRACE_MALLOC:
				printf("%p = malloc(%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,entry.call.infos.malloc.size,entry.call.timestamp,(int)entry.call.threadId);
				checkerTrace.registerSegment((void*)entry.result,entry.call.infos.malloc.size);
				ptr = alloc.malloc(entry.call.infos.malloc.size);
				printf("          => %p , %lu\n",ptr,alloc.getTotalSize(ptr));
				ptrs[entry.result] = ptr;
				checker.registerSegment(ptr,alloc.getTotalSize(ptr));
				break;
			case TRACE_FREE:
				ptr = ptrs[entry.call.infos.free.ptr];
				printf("free(%p) //timestamp = %lu, thread = %d, realptr = %p\n",
					   (void*)entry.call.infos.free.ptr,entry.call.timestamp,(int)entry.call.threadId,ptr);
				checkerTrace.unregisterSegment((void*)entry.call.infos.free.ptr,1);
				checker.unregisterSegment(ptr,alloc.getTotalSize(ptr));
				alloc.free(ptr);
				break;
			case TRACE_CALLOC:
				printf("%p = calloc(%lu,%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,entry.call.infos.calloc.nmemb,entry.call.infos.calloc.size,entry.call.timestamp,(int)entry.call.threadId);
				checkerTrace.registerSegment((void*)entry.result,entry.call.infos.calloc.nmemb*entry.call.infos.calloc.size);
				ptr = alloc.calloc(entry.call.infos.calloc.nmemb,entry.call.infos.calloc.size);
				printf("          => %p , %lu\n",ptr,alloc.getTotalSize(ptr));
				ptrs[entry.result] = ptr;
				checker.registerSegment(ptr,alloc.getTotalSize(ptr));
				break;
			case TRACE_REALLOC:
				ptr = ptrs[entry.call.infos.realloc.oldPtr];
				printf("%p = relloc(%p,%lu) //timestamp = %lu, thread = %d, realptr = %p\n",
					   (void*)entry.result,(void*)entry.call.infos.realloc.oldPtr,entry.call.infos.realloc.newSize,entry.call.timestamp,(int)entry.call.threadId,ptr);
				checkerTrace.unregisterSegment((void*)entry.call.infos.realloc.oldPtr,1);
				checkerTrace.registerSegment((void*)entry.result,entry.call.infos.realloc.newSize);
				checker.unregisterSegment(ptr,alloc.getTotalSize(ptr));
				ptr = alloc.realloc(ptr,entry.call.infos.realloc.newSize);
				printf("          => %p , %lu\n",ptr,alloc.getTotalSize(ptr));
				ptrs[entry.result] = ptr;
				checker.registerSegment(ptr,alloc.getTotalSize(ptr));
				break;
			case TRACE_MEMALIGN:
			case TRACE_ALIGN_ALLOC:
			case TRACE_POSIX_MEMALIGN:
				printf("%p = memalign(%lu,%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,entry.call.infos.memalign.align,entry.call.infos.memalign.size,entry.call.timestamp,(int)entry.call.threadId);
				checkerTrace.registerSegment((void*)entry.result,entry.call.infos.memalign.size);
				ptr = alloc.memalign(entry.call.infos.memalign.align,entry.call.infos.memalign.size);
				printf("          => %p , %lu\n",ptr,alloc.getTotalSize(ptr));
				ptrs[entry.result] = ptr;
				checker.registerSegment(ptr,alloc.getTotalSize(ptr));
				break;
			case TRACE_PVALLOC:
			case TRACE_VALLOC:
				printf("pvalloc/valloc\n");
				assert(false);
				break;
			default:
				fprintf(stderr,"Invalid event type %d\n",entry.call.type);
				abort();
		}
	}
	
	return EXIT_SUCCESS;
}
