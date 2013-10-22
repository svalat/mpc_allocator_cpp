/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <HugeAllocator.h>
#include <PaddedChunk.h>
#include <gtest/gtest.h>
#include "mocks/MockMMSource.h"
#include <RegionRegistry.h>

/**********************  USING  *********************/
using namespace testing;
using namespace MPCAllocator;

/********************  GLOBALS  *********************/
static char gblBuffer[REGION_SPLITTING*3];

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,constructor)
{
	MockMMSource source;
	HugeAllocator alloc(&source);
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,malloc)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);

	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	EXPECT_CALL(mm,map(1024,_,&alloc)).WillOnce(Return(segment));
	
	void * ptr = alloc.malloc(1024);

	EXPECT_EQ(segment+1,ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,free)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);

	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	EXPECT_CALL(mm,map(1024,_,&alloc)).WillOnce(Return(segment));
	EXPECT_CALL(mm,unmap(segment)).Times(1);
	
	void * ptr = alloc.malloc(1024);
	EXPECT_EQ(segment+1,ptr);
	alloc.free(ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,freeNull)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);
	
	alloc.free(NULL);
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,realloc_basic)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);
	const Size s1 = sizeof(gblBuffer)/3;
	const Size s2 = sizeof(gblBuffer) - s1;

	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,s1,&alloc);
	RegionSegmentHeader * segment2 = RegionSegmentHeader::setup(gblBuffer+s1,s2,&alloc);
	EXPECT_CALL(mm,map(1024,_,&alloc)).WillOnce(Return(segment));
	EXPECT_CALL(mm,remap(segment,s2 - sizeof(RegionSegmentHeader),&alloc)).WillOnce(Return(segment2));
	EXPECT_CALL(mm,unmap(segment2)).Times(1);
	
	void * ptr = alloc.malloc(1024);
	EXPECT_EQ(segment+1,ptr);
	
	ptr = alloc.realloc(ptr,s2 - sizeof(RegionSegmentHeader));
	EXPECT_EQ(segment2+1,ptr);
	
	alloc.free(ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,realloc_free)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);

	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	EXPECT_CALL(mm,map(1024,_,&alloc)).WillOnce(Return(segment));
	EXPECT_CALL(mm,unmap(segment)).Times(1);
	
	void * ptr = alloc.malloc(1024);
	EXPECT_EQ(segment+1,ptr);
	alloc.realloc(ptr,0);
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,realloc_malloc)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);

	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	EXPECT_CALL(mm,map(1024,_,&alloc)).WillOnce(Return(segment));
	
	void * ptr = alloc.realloc(NULL,1024);

	EXPECT_EQ(segment+1,ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,getInnerSize)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);

	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	EXPECT_CALL(mm,map(1024,_,&alloc)).WillOnce(Return(segment));
	
	void * ptr = alloc.malloc(1024);
	EXPECT_EQ(sizeof(gblBuffer)-sizeof(RegionSegmentHeader),alloc.getInnerSize(ptr));
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,getTotalSize)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);

	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	EXPECT_CALL(mm,map(1024,_,&alloc)).WillOnce(Return(segment));
	
	void * ptr = alloc.malloc(1024);
	EXPECT_EQ(sizeof(gblBuffer),alloc.getTotalSize(ptr));
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,testMallocAlign)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);
	
	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	EXPECT_CALL(mm,map(_,_,&alloc)).WillRepeatedly(Return(segment));

	for (int i = 8 ; i < 256 ; i+=4)
	{
		void * ptr = alloc.malloc(1024,i);
		ASSERT_NE((void*)NULL,ptr);
		ASSERT_EQ(0u,(Addr)ptr % i);
	}
	
	for (int i = 256 ; i < 4096 ; i+=32)
	{
		void * ptr = alloc.malloc(1024,i);
		ASSERT_NE((void*)NULL,ptr);
		ASSERT_EQ(0u,(Addr)ptr % i);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestHugeAllocator,testMallocFreeAlign)
{
	MockMMSource mm;
	HugeAllocator alloc(&mm);
	
	RegionSegmentHeader * segment = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&alloc);
	EXPECT_CALL(mm,map(_,_,&alloc)).WillRepeatedly(Return(segment));
	EXPECT_CALL(mm,unmap(segment)).Times(AtLeast(1));

	for (int i = 8 ; i < 256 ; i+=4)
	{
		void * ptr = alloc.malloc(1024,i);
		ASSERT_NE((void*)NULL,ptr);
		ASSERT_EQ(0u,(Addr)ptr % i);
		alloc.free(ptr);
	}
	
	for (int i = 256 ; i < 4096 ; i+=32)
	{
		void * ptr = alloc.malloc(1024,i);
		ASSERT_NE((void*)NULL,ptr);
		ASSERT_EQ(0u,(Addr)ptr % i);
		alloc.free(ptr);
	}
}
