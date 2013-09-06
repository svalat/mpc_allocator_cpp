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
		bool readNext(TraceEntryComplete & entry);
	private:
		FILE * fp;
};

#endif //ALLOC_TRACE_READER_H
