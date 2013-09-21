/********************  HEADERS  *********************/
#include <MediumAllocator.h>
#include <gtest/gtest.h>
#include "mocks/MockMMSource.h"
#include <RegionRegistry.h>

/**********************  USING  *********************/
using namespace testing;
using namespace MPCAllocator;

/********************  GLOBALS  *********************/
static char gblBuffer[REGION_SPLITTING];

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
	alloc.fill(buffer,sizeof(buffer),NULL);
	EXPECT_EQ(buffer+sizeof(MediumChunk),alloc.malloc(32));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,fillRegister)
{
	RegionRegistry region;
	MediumAllocator alloc;

	EXPECT_EQ(NULL,alloc.malloc(32));
	alloc.fill(gblBuffer,sizeof(gblBuffer),&region);
	void * ptr = alloc.malloc(32);
	EXPECT_EQ(gblBuffer+sizeof(MediumChunk) + sizeof(RegionSegmentHeader),ptr);
	EXPECT_EQ(gblBuffer,(void*)region.getSegment(ptr));
	EXPECT_EQ(&alloc,region.getSegment(ptr)->getManager());
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,malloc)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	EXPECT_EQ(NULL,alloc.malloc(32));
	alloc.fill(buffer,sizeof(buffer),NULL);
	EXPECT_EQ(buffer+sizeof(MediumChunk),alloc.malloc(32));
	EXPECT_EQ(buffer+sizeof(MediumChunk)*2 +32,alloc.malloc(32));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,free)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer),NULL);
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
	
	alloc.fill(buffer,sizeof(buffer),NULL);
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
	
	alloc.fill(buffer,sizeof(buffer),NULL);
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
	
	alloc.fill(buffer,sizeof(buffer),NULL);
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
	
	alloc.fill(buffer,sizeof(buffer),NULL);
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
	
	alloc.fill(buffer,sizeof(buffer),NULL);
	void * ptr = alloc.malloc(32);
	alloc.free(ptr);

	EXPECT_EQ(ptr,alloc.realloc(NULL,32));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,realloc_not_enought_mem)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer),NULL);
	void * ptr = alloc.malloc(32);
	ptr = alloc.realloc(ptr,10000);

	EXPECT_EQ(NULL,ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,realloc_copy)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer),NULL);
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
	
	alloc.fill(buffer,sizeof(buffer),NULL);
	char * ptr = (char*)alloc.malloc(32);
	
	EXPECT_EQ(32+sizeof(MediumChunk),alloc.getTotalSize(ptr));
	EXPECT_EQ(0,alloc.getTotalSize(NULL));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,getInnerSize)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer),NULL);
	char * ptr = (char*)alloc.malloc(32);
	
	EXPECT_EQ(32,alloc.getInnerSize(ptr));
	EXPECT_EQ(0,alloc.getInnerSize(NULL));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,getRequestedSize)
{
	MediumAllocator alloc;
	char buffer[4096];
	
	alloc.fill(buffer,sizeof(buffer),NULL);
	char * ptr = (char*)alloc.malloc(32);
	
	EXPECT_EQ(UNSUPPORTED,alloc.getRequestedSize(ptr));
	EXPECT_EQ(UNSUPPORTED,alloc.getRequestedSize(NULL));
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,testMemorySourceRefill)
{
	MockMMSource mm;
	MediumAllocator alloc(false,&mm);
	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	
	EXPECT_CALL(mm,map(32,_,&alloc)).WillOnce(Return(segment));
	
	void * ptr = alloc.malloc(32);
	EXPECT_EQ((char*)segment->getPtr()+sizeof(MediumChunk),ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,testMemorySourceFree)
{
	MockMMSource mm;
	MediumAllocator alloc(false,&mm);
	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	
	EXPECT_CALL(mm,map(32,_,&alloc)).WillOnce(Return(segment));
	
	void * ptr = alloc.malloc(32);
	EXPECT_EQ((char*)segment->getPtr()+sizeof(MediumChunk),ptr);
	
	EXPECT_CALL(mm,unmap(segment)).Times(1);
	alloc.free(ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,testMallocAlign_1)
{
	MediumAllocator alloc;
	alloc.fill(gblBuffer,sizeof(gblBuffer),NULL);

	for (int i = 8 ; i < 256 ; i+=4)
	{
		void * ptr = alloc.malloc(32,i);
		ASSERT_NE((void*)NULL,ptr);
		ASSERT_EQ(0,(Addr)ptr % i);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,testFreeAlign_1)
{
	MediumAllocator alloc;
	alloc.fill(gblBuffer,sizeof(gblBuffer),NULL);

	for (int i = 8 ; i < 256 ; i+=4)
	{
		void * ptr = alloc.malloc(32,i);
		ASSERT_NE((void*)NULL,ptr);
		ASSERT_EQ(0,(Addr)ptr % i);
		alloc.free(ptr);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestMediumAllocator,testMallocFreeAlign_2)
{
	MediumAllocator alloc;
	alloc.fill(gblBuffer,sizeof(gblBuffer),NULL);

	for (int i = 256 ; i < 4096 ; i+=32)
	{
		void * ptr = alloc.malloc(32,i);
		ASSERT_NE((void*)NULL,ptr);
		ASSERT_EQ(0,(Addr)ptr % i);
		alloc.free(ptr);
	}
}
