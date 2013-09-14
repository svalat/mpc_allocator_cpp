/********************  HEADERS  *********************/
#include <CachedMMSource.h>
#include <RegionRegistry.h>
#include <gtest/gtest.h>
#include "mocks/MockChunkManager.h"

/*******************  FUNCTION  *********************/
#define SIZE (1024*1024)

/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,freeBlocHandler)
{
	FreeMacroBloc bloc(1024);
	
	ListElement * element = bloc.getListHandler();
	FreeMacroBloc * bloc2 = FreeMacroBloc::getFromListHandler(element);
	
	EXPECT_EQ(&bloc,bloc2);
}

/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,constructor)
{
	RegionRegistry reg;
	CachedMMSource mm(&reg);
}

/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,mapWithReg)
{
	RegionRegistry reg;
	CachedMMSource mm(&reg);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	
	ASSERT_NE((void*)NULL,segment);
	EXPECT_EQ(REGION_SPLITTING,segment->getTotalSize());
	
	EXPECT_EQ(segment,reg.getSegment(segment+10));
}

/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,mapWithoutReg)
{
	CachedMMSource mm(NULL);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	
	ASSERT_NE((void*)NULL,segment);
	EXPECT_EQ(REGION_SPLITTING,segment->getTotalSize());
	EXPECT_EQ(&manager,segment->getManager());
}

/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,unmapWithReg)
{
	RegionRegistry reg;
	CachedMMSource mm(&reg);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	mm.unmap(segment);
	
	EXPECT_EQ(NULL,reg.getSegment(segment+10));
}
 
/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,unmapWithoutReg)
{
	CachedMMSource mm(NULL);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	mm.unmap(segment);
}
 
/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,remapWithReg)
{
	RegionRegistry reg;
	CachedMMSource mm(&reg);
	MockChunkManager manager;
	
	//TODO find a way to mock the registry
	//TODO try with splitting to ensure move and check registry remove
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	mm.map(SIZE,NULL,&manager);
	RegionSegmentHeader * segment2 = mm.remap(segment,2*SIZE,&manager);
	
	EXPECT_EQ((void*)NULL,reg.getSegment(segment+10));
	EXPECT_EQ(segment2,reg.getSegment(segment2+10));
}
 
/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,remapWithoutReg)
{
	CachedMMSource mm(NULL);
	MockChunkManager manager;
	
	RegionSegmentHeader * segment = mm.map(SIZE,NULL,&manager);
	mm.remap(segment,2*SIZE,&manager);
}

/*******************  FUNCTION  *********************/
static void markContent(void * ptr,size_t size,unsigned char tag,size_t skip = 0)
{
	unsigned char * tmp = (unsigned char *)ptr;
	for (size_t i = skip ; i < size ; i++)
		tmp[i] = tag;
}

/*******************  FUNCTION  *********************/
static void checkContent(void * ptr,size_t size,unsigned char tag,size_t skip = 0)
{
	unsigned char * tmp = (unsigned char *)ptr;
	for (size_t i = skip ; i < size ; i++)
		ASSERT_EQ((int)tag,(int)tmp[i]);
}

/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,reuseSplit_1)
{
	CachedMMSource mm(NULL,8*REGION_SPLITTING,4*REGION_SPLITTING,true);
	MockChunkManager manager;
	
	RegionSegmentHeader * segmentOrig = mm.map(2*REGION_SPLITTING,NULL,&manager);
	markContent(segmentOrig->getPtr(),segmentOrig->getInnerSize(),42);
	mm.unmap(segmentOrig);
	
	RegionSegmentHeader * segment1 = mm.map(4,NULL,&manager);
	RegionSegmentHeader * segment2 = mm.map(4,NULL,&manager);
	
	EXPECT_EQ(REGION_SPLITTING,segment1->getTotalSize());
	EXPECT_EQ(segmentOrig,segment1);
	checkContent(segment1->getPtr(),segment1->getInnerSize(),42,64);
	
	EXPECT_EQ(REGION_SPLITTING,segment2->getTotalSize());
	EXPECT_EQ(addrOffset(segmentOrig,REGION_SPLITTING),segment2);
	checkContent(segment2->getPtr(),segment2->getInnerSize(),42,64);
}

/*******************  FUNCTION  *********************/
TEST(TestCachedMMSource,reuseSplit_2)
{
	CachedMMSource mm(NULL,8*REGION_SPLITTING,4*REGION_SPLITTING,false);
	MockChunkManager manager;
	
	RegionSegmentHeader * segmentOrig = mm.map(2*REGION_SPLITTING,NULL,&manager);
	markContent(segmentOrig->getPtr(),segmentOrig->getInnerSize(),42);
	mm.unmap(segmentOrig);
	
	RegionSegmentHeader * segment1 = mm.map(4,NULL,&manager);
	RegionSegmentHeader * segment2 = mm.map(4,NULL,&manager);
	
	EXPECT_EQ(REGION_SPLITTING,segment1->getTotalSize());
	EXPECT_EQ(segmentOrig,segment1);
	checkContent(segment1->getPtr(),segment1->getInnerSize(),42,64);
	
	EXPECT_EQ(REGION_SPLITTING,segment2->getTotalSize());
	checkContent(segment2->getPtr(),segment2->getInnerSize(),0);
}
