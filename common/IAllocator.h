#ifndef IALLOCATOR_H
#define IALLOCATOR_H

#include <stdlib.h>

class IChunkManager
{
	public:
		virtual void free(void * ptr) = 0;
		virtual void * realloc(void * ptr,size_t size) = 0;
		virtual size_t getInnerSize(void * ptr) = 0;
		virtual size_t getTotalSize(void * ptr) = 0;
		virtual size_t getRequestedSize(void * ptr) = 0;
};

class IAllocator : public IChunkManager
{
	public:
		virtual void * malloc(size_t size,size_t align,bool * zeroFilled = NULL) = 0;
};

#endif //IALLOCATOR_H
