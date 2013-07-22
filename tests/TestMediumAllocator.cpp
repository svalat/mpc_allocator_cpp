/********************  HEADERS  *********************/
#include <MediumAllocator.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace testing;

/*********************  CLASS  **********************/
class MockMMSrc : public IAllocator {
	public:
		MOCK_METHOD1(free,void( void* ptr ));
		MOCK_METHOD1(getInnerSize, size_t( void* ptr ));
		MOCK_METHOD1(getRequestedSize,size_t( void* ptr ));
		MOCK_METHOD3(malloc,void* ( size_t size, size_t align, bool* zeroFilled ));
		MOCK_METHOD1(getTotalSize, size_t( void* ptr ));
		MOCK_METHOD2(realloc,void* ( void* ptr, size_t size ));
};

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,constructor)
{
	MediumAllocator alloc;
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,fill)
{
	MediumAllocator alloc;
	char buffer[4096];

	EXPECT_EQ(NULL,alloc.malloc(32));
	alloc.fill(buffer,sizeof(buffer));
	EXPECT_EQ(buffer+sizeof(MediumChunk),alloc.malloc(32));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,malloc)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	EXPECT_EQ(NULL,alloc.malloc(32));
	alloc.fill(buffer,sizeof(buffer));
	EXPECT_EQ(buffer+sizeof(MediumChunk),alloc.malloc(32));
	EXPECT_EQ(buffer+sizeof(MediumChunk)*2 +32,alloc.malloc(32));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,free)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	void * ptr = alloc.malloc(32);
	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
	alloc.free(ptr);
	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
	EXPECT_EQ(buffer+sizeof(MediumChunk),alloc.malloc(32));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,realloc_basic)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	void * ptr = alloc.malloc(32);
	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
	ptr = alloc.realloc(ptr,16);
	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
	EXPECT_EQ(32,alloc.getInnerSize(ptr));
}

/*******************  FUNCTION  *********************/
/** Here we need to work to use some merging stuff with next element if free **/
TEST(TestMediumAllocator,realloc_to_small_NEED_SOME_WORK)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	void * ptr = alloc.malloc(32);
	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
	ptr = alloc.realloc(ptr,64);
	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
	EXPECT_EQ(64,alloc.getInnerSize(ptr));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,realloc_move)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	void * ptr = alloc.malloc(32);
	alloc.malloc(32);
	void * ptr2 = alloc.malloc(64);
	alloc.free(ptr2);

	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
	ptr = alloc.realloc(ptr,64);
	EXPECT_EQ(ptr2,ptr);
	EXPECT_EQ(64,alloc.getInnerSize(ptr));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,realloc_free)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	void * ptr = alloc.malloc(32);

	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
	EXPECT_EQ(NULL,alloc.realloc(ptr,0));
	EXPECT_EQ(ptr,alloc.malloc(32));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,realloc_malloc)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	void * ptr = alloc.malloc(32);
	alloc.free(ptr);

	EXPECT_EQ(ptr,alloc.realloc(NULL,32));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,realloc_not_enought_mem)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	void * ptr = alloc.malloc(32);
	ptr = alloc.realloc(ptr,10000);

	EXPECT_EQ(NULL,ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,realloc_copy)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	char * ptr = (char*)alloc.malloc(32);
	alloc.malloc(32);
	
	for (int i = 0 ; i < 32 ; i++)
		ptr[i] = i;
	
	ptr = (char*)alloc.realloc(ptr,64);
	
	for (int i = 0 ; i < 32 ; i++)
		EXPECT_EQ(i,ptr[i]);
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,getTotalSize)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	char * ptr = (char*)alloc.malloc(32);
	
	EXPECT_EQ(32+sizeof(MediumChunk),alloc.getTotalSize(ptr));
	EXPECT_EQ(0,alloc.getTotalSize(NULL));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,getInnerSize)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	char * ptr = (char*)alloc.malloc(32);
	
	EXPECT_EQ(32,alloc.getInnerSize(ptr));
	EXPECT_EQ(0,alloc.getInnerSize(NULL));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,getRequestedSize)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer));
	char * ptr = (char*)alloc.malloc(32);
	
	EXPECT_EQ(UNSUPPORTED,alloc.getRequestedSize(ptr));
	EXPECT_EQ(UNSUPPORTED,alloc.getRequestedSize(NULL));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,testMemorySourceRefill)
{
	char buffer[4096];
	MockMMSrc mm;
	MediumAllocator alloc(false,&mm);
	
	EXPECT_CALL(mm,malloc(32,BASIC_ALIGN,_)).WillOnce(Return(buffer));
	EXPECT_CALL(mm,getInnerSize(buffer)).WillOnce(Return(sizeof(buffer)));
	
	void * ptr = alloc.malloc(32);
	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,testMemorySourceFree)
{
	char buffer[4096];
	MockMMSrc mm;
	MediumAllocator alloc(false,&mm);
	
	EXPECT_CALL(mm,malloc(32,BASIC_ALIGN,_)).WillOnce(Return(buffer));
	EXPECT_CALL(mm,getInnerSize(buffer)).WillOnce(Return(sizeof(buffer)));
	
	void * ptr = alloc.malloc(32);
	EXPECT_EQ(buffer+sizeof(MediumChunk),ptr);
	
	EXPECT_CALL(mm,free(buffer)).Times(1);
	alloc.free(ptr);
}
