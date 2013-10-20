#ifndef ALLOC_TRACE_READER_H
#define ALLOC_TRACE_READER_H

/********************  HEADERS  *********************/
#include <cstdio>
#include "AllocTraceStruct.h"

/*********************  CLASS  **********************/
class TraceReader
{
	public:
		TraceReader(const char * fname);
		~TraceReader(void);
		bool readNext(MPCAllocator::TraceEntryComplete & entry);
		bool readPrev(MPCAllocator::TraceEntryComplete & entry);
		size_t getCurrentId(void) const;
		void setPosition(size_t id);
		void reopen(void);
	private:
		FILE * fp;
		size_t pos;
		const char * name;
};

#endif //ALLOC_TRACE_READER_H
