#ifndef DUMMY_MM_SOURCE_H
#define DUMMY_MM_SOURCE_H

/********************  HEADERS  *********************/
#include <IAllocator.h>

/*******************  FUNCTION  *********************/
class DummyMMSource : public IAllocator
{
	public:
		virtual size_t getTotalSize ( void* ptr );
		virtual size_t getRequestedSize ( void* ptr );
		virtual size_t getInnerSize ( void* ptr );
		virtual void free ( void* ptr );
		virtual void* malloc ( size_t size, size_t align, bool* zeroFilled = 0 );
		virtual void* realloc ( void* ptr, size_t size );
};

#endif //DUMMY_MM_SOURCE_H
