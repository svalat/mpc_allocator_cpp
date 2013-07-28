#ifndef MOCK_ALLOCATOR_H
#define MOCK_ALLOCATOR_H

/********************  HEADERS  *********************/
#include <IAllocator.h>
#include <gmock/gmock.h>

/*********************  CLASS  **********************/
class MockAllocator : public IAllocator {
	public:
		MOCK_METHOD1(free,void( void* ptr ));
		MOCK_METHOD1(getInnerSize, size_t( void* ptr ));
		MOCK_METHOD1(getRequestedSize,size_t( void* ptr ));
		MOCK_METHOD3(malloc,void* ( size_t size, size_t align, bool* zeroFilled ));
		MOCK_METHOD1(getTotalSize, size_t( void* ptr ));
		MOCK_METHOD2(realloc,void* ( void* ptr, size_t size ));
};

#endif //MOCK_ALLOCATOR_H
