/********************  HEADERS  *********************/
#include <SmallAllocator.h>
#include <OS.h>
#include "mocks/MockMMSource.h"
#include <gtest/gtest.h>

/**********************  USING  *********************/
using namespace testing;

/********************  GLOBALS  *********************/
static char gblBuffer[REGION_SPLITTING];

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,constructor)
{
	SmallAllocator alloc1();
	SmallAllocator alloc2(false);
	SmallAllocator alloc3(false,NULL);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,mallocEmpty)
{
	SmallAllocator alloc;
	EXPECT_EQ(NULL,alloc.malloc(16));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,mallocFill)
{
	SmallAllocator alloc;
	void * mem = OS::mmap(NULL,PAGE_SIZE);
	EXPECT_NE((void*)NULL,mem);
	alloc.fill(mem,PAGE_SIZE,NULL);
	void * ptr = alloc.malloc(16);
	EXPECT_NE((void*)NULL,ptr);
	EXPECT_GE(ptr,mem);
	EXPECT_LT(ptr,(char*)mem+PAGE_SIZE);
	
	OS::munmap(mem,PAGE_SIZE);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,mallocFillALL)
{
	SmallAllocator alloc;
	void * mem = OS::mmap(NULL,PAGE_SIZE);
	EXPECT_NE((void*)NULL,mem);
	alloc.fill(mem,PAGE_SIZE,NULL);
	
	void * ptr;
	do
	{
		ptr = alloc.malloc(16);
		if (ptr != NULL)
		{
			EXPECT_NE((void*)NULL,ptr);
			EXPECT_GE(ptr,mem);
			EXPECT_LT(ptr,(char*)mem+PAGE_SIZE);
		}
	} while(ptr != NULL);
	
	OS::munmap(mem,PAGE_SIZE);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,mallocFillALLMultiPage)
{
	SmallAllocator alloc;
	void * mem = OS::mmap(NULL,2*PAGE_SIZE);
	EXPECT_NE((void*)NULL,mem);
	alloc.fill(mem,2*PAGE_SIZE,NULL);
	
	void * ptr;
	do
	{
		ptr = alloc.malloc(16);
		if (ptr != NULL)
		{
			EXPECT_NE((void*)NULL,ptr);
			EXPECT_GE(ptr,mem);
			EXPECT_LT(ptr,(char*)mem+2*PAGE_SIZE);
		}
	} while(ptr != NULL);
	
	OS::munmap(mem,2*PAGE_SIZE);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,fillNonAligned)
{
	SmallAllocator alloc;
	unsigned char * mem = (unsigned char*)OS::mmap(NULL,PAGE_SIZE);
	EXPECT_NE((void*)NULL,mem);
	
	for (int i = 0 ; i < 32 ; i++)
		mem[i] = i;
	
	alloc.fill((char*)mem+32,PAGE_SIZE-32,NULL);
	
	EXPECT_NE((void*)NULL,alloc.malloc(16));
	void * ptr;
	while((ptr = alloc.malloc(16)) != NULL)
	{
		memset(ptr,0,16);
	}
	
	for (int i = 0 ; i < 32 ; i++)
		EXPECT_EQ(i,mem[i]);
	
	OS::munmap(mem,PAGE_SIZE);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,fillRegistry)
{
	RegionRegistry registry;
	SmallAllocator alloc;
	void * mem = OS::mmap(NULL,REGION_SPLITTING);
	
	EXPECT_EQ(NULL,registry.getSegment(mem));
	alloc.fill(mem,REGION_SPLITTING,&registry);
	ASSERT_EQ(mem,registry.getSegment(mem));
	EXPECT_EQ(REGION_SPLITTING,registry.getSegment(mem)->getTotalSize());
	
	//try to use it
	void * ptr;
	int cnt = 0;
	while((ptr = alloc.malloc(16)) != NULL)
	{
		cnt++;
		ASSERT_TRUE(ptr > mem && ptr < (char*)mem + REGION_SPLITTING);
		ASSERT_NE((void*)NULL,ptr);
		ASSERT_EQ(mem,registry.getSegment(ptr));
	}
	
	OS::munmap(mem,REGION_SPLITTING);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,refill)
{
	MockMMSource mm;
	SmallAllocator alloc(false,&mm);
	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	
	EXPECT_CALL(mm,map(REGION_SPLITTING-sizeof(RegionSegmentHeader),NULL,&alloc)).WillOnce(Return(segment));
	
	EXPECT_NE((void*)NULL,alloc.malloc(16));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,refillOOM)
{
	MockMMSource mm;
	SmallAllocator alloc(false,&mm);
	
	EXPECT_CALL(mm,map(REGION_SPLITTING-sizeof(RegionSegmentHeader),NULL,&alloc)).WillOnce(Return((RegionSegmentHeader*)NULL));
	
	EXPECT_EQ((void*)NULL,alloc.malloc(16));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,free1)
{
	SmallAllocator alloc;
	alloc.fill(gblBuffer,sizeof(gblBuffer),NULL);
	
	void * ptr1 = alloc.malloc(16);
	alloc.free(ptr1);
	void * ptr2 = alloc.malloc(16);
	EXPECT_EQ(ptr1,ptr2);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallAllocator,real_free)
{
	MockMMSource mm;
	SmallAllocator alloc(false,&mm);

	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	Expectation op1 = EXPECT_CALL(mm,map(REGION_SPLITTING-sizeof(RegionSegmentHeader),NULL,&alloc)).WillOnce(Return(segment));
	EXPECT_CALL(mm,unmap(segment)).After(op1);
	
	void * ptr = alloc.malloc(16);
	alloc.free(ptr);
}
