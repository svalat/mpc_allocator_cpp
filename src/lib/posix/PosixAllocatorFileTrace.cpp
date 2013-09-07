/********************  HEADERS  *********************/
#include <cstdio>
#include "OS.h"
#include "Debug.h"
#include "PosixAllocatorFileTrace.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CONSTS  *********************/
static const char cstTraceFilename[] = "alloc-trace-%08d.raw";

/********************  GLOBALS  *********************/
static __thread int gblThreadId = 0;
static bool gblInit = false;

/*******************  FUNCTION  *********************/
PosixAllocatorFileTrace::PosixAllocatorFileTrace ( void ) 
{
	//allocate tmp buffer
	char fname[2*sizeof(cstTraceFilename)];
	
	//errors
	allocAssert(gblInit == false);
	nextThreadId = 1;

	//get pid
	int pid = OS::getpid();

	//build fname
	size_t res = sprintf(fname,cstTraceFilename,pid);
	allocAssert(res <= sizeof(fname));

	//open file
	allocDebug("Trace allocator in raw file %s",fname);
	fd = OS::openFileDescriptor(fname,true);
	allocAssumePerror(fd >= 0,"Failed to open trace file");
	
	//mark init ok (for debug)
	gblInit = true;
}

/*******************  FUNCTION  *********************/
void PosixAllocatorFileTrace::writeEvent ( TraceEntry& entry, TraceEntryType type )
{
	//init
	entry.timestamp = getticks();
	entry.type = type;
	entry.threadId = gblThreadId;
	
	//check for threadId init
	if (entry.threadId <= 0)
	{
		allocDebug("Alloc trace new thread %d\n",nextThreadId);
		entry.threadId = nextThreadId++;
		gblThreadId = entry.threadId;
	}
	
	//write
	allocAssert(fd >= 0);
	size_t status = OS::safeWrite(fd,&entry,sizeof(entry));
	allocAssert(status == sizeof(entry));
}

/*******************  FUNCTION  *********************/
void PosixAllocatorFileTrace::writeAnswer ( void* res )
{
	//tmp
	uint64_t formatedRes = (uint64_t)res;
	//write
	allocAssert(fd >= 0);
	size_t status = OS::safeWrite(fd,&formatedRes,sizeof(formatedRes));
	allocAssert(status == sizeof(formatedRes));
}


/*******************  FUNCTION  *********************/
void* PosixAllocatorFileTrace::malloc ( size_t size )
{
	//generate
	TraceEntry event;
	void * res = NULL;
	
	//check
	allocAssert(gblInit);
	
	//special info
	event.infos.malloc.size = size;
	
	START_CRITICAL(lock);
		writeEvent(event,TRACE_MALLOC);
		res = super::malloc(size);
		writeAnswer(res);
	END_CRITICAL;
	
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorFileTrace::aligned_alloc ( size_t alignment, size_t size )
{
	//generate
	TraceEntry event;
	void * res = NULL;
	
	//check
	allocAssert(gblInit);
	
	//special info
	event.infos.aligned_alloc.align = alignment;
	event.infos.aligned_alloc.size = size;
	
	START_CRITICAL(lock);
		writeEvent(event,TRACE_ALIGN_ALLOC);
		res = super::aligned_alloc(alignment,size);
		writeAnswer(res);
	END_CRITICAL;
	
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorFileTrace::calloc ( size_t nmemb, size_t size )
{
	//generate
	TraceEntry event;
	void * res = NULL;
	
	//check
	allocAssert(gblInit);
	
	//special info
	event.infos.calloc.nmemb = nmemb;
	event.infos.calloc.size = size;
	
	START_CRITICAL(lock);
		writeEvent(event,TRACE_CALLOC);
		res = super::calloc(nmemb,size);
		writeAnswer(res);
	END_CRITICAL;
	
	return res;
}

/*******************  FUNCTION  *********************/
void PosixAllocatorFileTrace::free ( void* ptr )
{
	//generate
	TraceEntry event;
	
	//check
	allocAssert(gblInit);
	
	//trivial
	if (ptr == NULL)
		return;
	
	//special info
	event.infos.free.ptr = (uint64_t)ptr;
	
	START_CRITICAL(lock);
		writeEvent(event,TRACE_FREE);
		super::free(ptr);
		writeAnswer(NULL);
	END_CRITICAL;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorFileTrace::memalign ( size_t alignment, size_t size )
{
	//generate
	TraceEntry event;
	void * res = NULL;
	
	//check
	allocAssert(gblInit);
	
	//special info
	event.infos.memalign.align = alignment;
	event.infos.memalign.size = size;
	
	START_CRITICAL(lock);
		writeEvent(event,TRACE_MEMALIGN);
		res = super::memalign(alignment,size);
		writeAnswer(res);
	END_CRITICAL;
	
	return res;
}

/*******************  FUNCTION  *********************/
int PosixAllocatorFileTrace::posix_memalign ( void** memptr, size_t alignment, size_t size )
{
	//generate
	TraceEntry event;
	int res = -1;
	
	//check
	allocAssert(gblInit);
	
	//special info
	event.infos.posix_memalign.align = alignment;
	event.infos.posix_memalign.size = size;
	
	START_CRITICAL(lock);
		writeEvent(event,TRACE_POSIX_MEMALIGN);
		res = super::posix_memalign(memptr,alignment,size);
		writeAnswer(*memptr);
	END_CRITICAL;
	
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorFileTrace::realloc ( void* ptr, size_t size )
{
	//generate
	TraceEntry event;
	void * res = NULL;
	
	//check
	allocAssert(gblInit);
	
	//special info
	event.infos.realloc.oldPtr = (uint64_t)ptr;
	event.infos.realloc.newSize = size;
	
	START_CRITICAL(lock);
		writeEvent(event,TRACE_REALLOC);
		res = super::realloc(ptr,size);
		writeAnswer(res);
	END_CRITICAL;
	
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorFileTrace::pvalloc ( size_t size )
{
	//generate
	TraceEntry event;
	void * res = NULL;
	
	//check
	allocAssert(gblInit);
	
	//special info
	event.infos.pvalloc.size = size;
	
	START_CRITICAL(lock);
		writeEvent(event,TRACE_PVALLOC);
		res = super::pvalloc(size);
		writeAnswer(res);
	END_CRITICAL;
	
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorFileTrace::valloc ( size_t size )
{
	//generate
	TraceEntry event;
	void * res = NULL;
	
	//check
	allocAssert(gblInit);
	
	//special info
	event.infos.valloc.size = size;
	
	START_CRITICAL(lock);
		writeEvent(event,TRACE_VALLOC);
		res = super::valloc(size);
		writeAnswer(res);
	END_CRITICAL;
	
	return res;
}

};
