/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef ALLOC_TRACE_STRUC_H
#define ALLOC_TRACE_STRUC_H

/********************  HEADERS  *********************/
#include <cstdlib>
#include <cycle.h>
#include <stdint.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  STRUCT  *********************/
struct TraceMallocArgs
{
	size_t size;
};

/*********************  STRUCT  *********************/
struct TraceAlignAllocArgs
{
	size_t align;
	size_t size;
};

/*********************  STRUCT  *********************/
struct TraceCallocArgs
{
	size_t nmemb;
	size_t size;
};

/*********************  STRUCT  *********************/
struct TraceFreeArgs
{
	uint64_t ptr;
};

/*********************  STRUCT  *********************/
struct TraceReallocArgs
{
	uint64_t oldPtr;
	size_t newSize;
};

/********************  ENUM  ************************/
enum TraceEntryType
{
	TRACE_MALLOC,
	TRACE_ALIGN_ALLOC,
	TRACE_CALLOC,
	TRACE_FREE,
	TRACE_MEMALIGN,
	TRACE_POSIX_MEMALIGN,
	TRACE_REALLOC,
	TRACE_PVALLOC,
	TRACE_VALLOC,
};

/*********************  STRUCT  *********************/
struct TraceEntry
{
	uint64_t timestamp;
	union {
		TraceFreeArgs free;
		TraceMallocArgs malloc;
		TraceCallocArgs calloc;
		TraceReallocArgs realloc;
		TraceAlignAllocArgs aligned_alloc;
		TraceAlignAllocArgs memalign;
		TraceAlignAllocArgs posix_memalign;
		TraceMallocArgs pvalloc;
		TraceMallocArgs valloc;
	} infos;
	uint16_t threadId;
	uint16_t type;
};

/*********************  STRUCT  *********************/
struct TraceEntryComplete
{
	TraceEntry call;
	uint64_t result;
};

};

#endif //ALLOC_TRACE_STRUC_H
