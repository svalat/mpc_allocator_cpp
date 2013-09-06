/********************  HEADERS  *********************/
#include <cstdio>
#include <cassert>
#include "TraceReader.h"
#include <PosixAllocator.h>
#include <map>

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
	
	while (reader.readNext(entry))
	{
		switch(entry.call.type)
		{
			case TRACE_MALLOC:
				printf("%p = malloc(%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,entry.call.infos.malloc.size,entry.call.timestamp,(int)entry.call.threadId);
				ptrs[entry.result] = alloc.malloc(entry.call.infos.malloc.size);
				break;
			case TRACE_FREE:
				printf("free(%p) //timestamp = %lu, thread = %d\n",
					   (void*)entry.call.infos.free.ptr,entry.call.timestamp,(int)entry.call.threadId);
				alloc.free(ptrs[entry.call.infos.free.ptr]);
				break;
			case TRACE_CALLOC:
				printf("%p = calloc(%lu,%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,entry.call.infos.calloc.nmemb,entry.call.infos.calloc.size,entry.call.timestamp,(int)entry.call.threadId);
				ptrs[entry.result] = alloc.calloc(entry.call.infos.calloc.nmemb,entry.call.infos.calloc.size);
				break;
			case TRACE_REALLOC:
				printf("%p = relloc(%p,%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,(void*)entry.call.infos.realloc.oldPtr,entry.call.infos.realloc.newSize,entry.call.timestamp,(int)entry.call.threadId);
				ptrs[entry.result] = alloc.realloc((void*)entry.call.infos.realloc.oldPtr,entry.call.infos.realloc.newSize);
				break;
			case TRACE_MEMALIGN:
			case TRACE_ALIGN_ALLOC:
			case TRACE_POSIX_MEMALIGN:
				printf("%p = memalign(%lu,%lu) //timestamp = %lu, thread = %d\n",
					   (void*)entry.result,entry.call.infos.memalign.align,entry.call.infos.memalign.size,entry.call.timestamp,(int)entry.call.threadId);
				ptrs[entry.result] = alloc.memalign(entry.call.infos.memalign.align,entry.call.infos.memalign.size);
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