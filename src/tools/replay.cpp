/********************  HEADERS  *********************/
#include <cstdio>
#include <cassert>
#include <PosixAllocator.h>
#include <map>
#include "TraceReader.h"
#include "CollisionChecker.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	assert(argc == 2);
	TraceReader reader(argv[1]);
	TraceEntryComplete entry;
	PosixAllocator alloc;
	std::map<uint64_t,void*> ptrs;
	CollisionChecker checker;
	void * ptr;
	
	while (reader.readNext(entry))
	{
		switch(entry.call.type)
		{
			case TRACE_MALLOC:
				printf("%p = malloc(%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,entry.call.infos.malloc.size,entry.call.timestamp,(int)entry.call.threadId);
				ptr = alloc.malloc(entry.call.infos.malloc.size);
				printf("          => %p , %lu\n",ptr,alloc.getTotalSize(ptr));
				ptrs[entry.result] = ptr;
				checker.registerSegment(ptr,alloc.getTotalSize(ptr));
				break;
			case TRACE_FREE:
				printf("free(%p) //timestamp = %lu, thread = %d\n",
					   (void*)entry.call.infos.free.ptr,entry.call.timestamp,(int)entry.call.threadId);
				ptr = ptrs[entry.call.infos.free.ptr];
				alloc.free(ptr);
				checker.unregisterSegment(ptr,alloc.getTotalSize(ptr));
				break;
			case TRACE_CALLOC:
				printf("%p = calloc(%lu,%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,entry.call.infos.calloc.nmemb,entry.call.infos.calloc.size,entry.call.timestamp,(int)entry.call.threadId);
				ptr = alloc.calloc(entry.call.infos.calloc.nmemb,entry.call.infos.calloc.size);
				printf("          => %p , %lu\n",ptr,alloc.getTotalSize(ptr));
				ptrs[entry.result] = ptr;
				checker.registerSegment(ptr,alloc.getTotalSize(ptr));
				break;
			case TRACE_REALLOC:
				printf("%p = relloc(%p,%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,(void*)entry.call.infos.realloc.oldPtr,entry.call.infos.realloc.newSize,entry.call.timestamp,(int)entry.call.threadId);
				ptr = alloc.realloc((void*)entry.call.infos.realloc.oldPtr,entry.call.infos.realloc.newSize);
				printf("          => %p , %lu\n",ptr,alloc.getTotalSize(ptr));
				ptrs[entry.result] = ptr;
				checker.registerSegment(ptr,alloc.getTotalSize(ptr));
				break;
			case TRACE_MEMALIGN:
			case TRACE_ALIGN_ALLOC:
			case TRACE_POSIX_MEMALIGN:
				printf("%p = memalign(%lu,%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,entry.call.infos.memalign.align,entry.call.infos.memalign.size,entry.call.timestamp,(int)entry.call.threadId);
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
