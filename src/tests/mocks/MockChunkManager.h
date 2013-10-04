#ifndef MOCK_CHUNK_MANAGER_H
#define MOCK_CHUNK_MANAGER_H

/********************  HEADERS  *********************/
#include <IAllocator.h>
#include <gmock/gmock.h>

/**********************  USING  *********************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
class MockChunkManager : public IChunkManager {
	public:
		MOCK_METHOD1(free,void( void* ptr ));
		MOCK_METHOD1(getInnerSize, size_t( void* ptr ));
		MOCK_METHOD1(getRequestedSize,size_t( void* ptr ));
		MOCK_METHOD1(getTotalSize, size_t( void* ptr ));
		MOCK_METHOD2(realloc,void* ( void* ptr, size_t size ));
		MOCK_METHOD1(remoteFree,void(void* ptr));
		MOCK_CONST_METHOD0(isThreadSafe,bool(void));
};

#endif
