/********************  HEADERS  *********************/
#include <DummyMMSource.h>
#include <RegionRegistry.h>
#include <gtest/gtest.h>
#include "mocks/MockChunkManager.h"

/*******************  FUNCTION  *********************/
#define SIZE (1024*1024)

/*******************  FUNCTION  *********************/
TEST(TestDummyMMSource,constructor)
{
	RegionRegistry reg;
	DummyMMSource mm(&reg);
}

/*******************  FUNCTION  *********************/
TEST(TestDummyMMSource,mapWithReg)
{
	RegionRegistry reg;
	DummyMMSource mm(&reg);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	
	ASSERT_NE((void*)NULL,segment);
	EXPECT_EQ(REGION_SPLITTING,segment->getTotalSize());
	
	EXPECT_EQ(segment,reg.getSegment(segment+10));
}

/*******************  FUNCTION  *********************/
TEST(TestDummyMMSource,mapWithoutReg)
{
	DummyMMSource mm(NULL);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	
	ASSERT_NE((void*)NULL,segment);
	EXPECT_EQ(REGION_SPLITTING,segment->getTotalSize());
	EXPECT_EQ(&manager,segment->getManager());
}

/*******************  FUNCTION  *********************/
TEST(TestDummyMMSource,unmapWithReg)
{
	RegionRegistry reg;
	DummyMMSource mm(&reg);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	mm.unmap(segment);
	
	EXPECT_EQ(NULL,reg.getSegment(segment+10));
}

/*******************  FUNCTION  *********************/
TEST(TestDummyMMSource,unmapWithoutReg)
{
	DummyMMSource mm(NULL);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	mm.unmap(segment);
}

/*******************  FUNCTION  *********************/
TEST(TestDummyMMSource,remapWithReg)
{
	RegionRegistry reg;
	DummyMMSource mm(&reg);
	MockChunkManager manager;
	
	//TODO find a way to mock the registry
	//TODO try with splitting to ensure move and check registry remove
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	mm.map(SIZE,NULL,&manager);
	RegionSegmentHeader * segment2 = mm.remap(segment,2*SIZE,&manager);
	
	EXPECT_EQ(NULL,reg.getSegment(segment+10));
	EXPECT_EQ(segment2,reg.getSegment(segment2+10));
}

/*******************  FUNCTION  *********************/
TEST(TestDummyMMSource,remapWithoutReg)
{
	DummyMMSource mm(NULL);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	mm.remap(segment,2*SIZE,&manager);
}
