/********************  HEADERS  *********************/
#include <RegionRegistry.h>
#include <OS.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

/**********************  USING  *********************/
using namespace testing;

/********************  GLOBALS  *********************/
#define SIZE (4*1024*1024)
static char gblBuffer[SIZE];

/*********************  CLASS  **********************/
class MockChunkManager : public IChunkManager {
	public:
		MOCK_METHOD1(free,void( void* ptr ));
		MOCK_METHOD1(getInnerSize, size_t( void* ptr ));
		MOCK_METHOD1(getRequestedSize,size_t( void* ptr ));
		MOCK_METHOD1(getTotalSize, size_t( void* ptr ));
		MOCK_METHOD2(realloc,void* ( void* ptr, size_t size ));
};

/*******************  FUNCTION  *********************/
TEST(RegionSegmentHeader,setup)
{
	MockChunkManager manager;
	RegionSegmentHeader * res = RegionSegmentHeader::setup(gblBuffer,sizeof(gblBuffer),&manager);
	
	EXPECT_EQ(SIZE,res->getTotalSize());
	EXPECT_EQ(SIZE-sizeof(RegionSegmentHeader),res->getInnerSize());
	EXPECT_EQ(gblBuffer+sizeof(RegionSegmentHeader),res->getPtr());
	EXPECT_EQ(&manager,res->getManager());
}

/*******************  FUNCTION  *********************/
TEST(Region, constructor)
{
	Region region;
	region.clear();
	EXPECT_TRUE(region.isEmpty());
}

/*******************  FUNCTION  *********************/
TEST(RegionRegistry,constructor)
{
	RegionRegistry registry;
}

/*******************  FUNCTION  *********************/
TEST(RegionRegistry,seq1)
{
	RegionRegistry registry;
	
	EXPECT_EQ(NULL,registry.getSegment(gblBuffer));
	EXPECT_FALSE(registry.hasEntry(gblBuffer));
}

/*******************  FUNCTION  *********************/
TEST(RegionRegistry,seq3)
{
	RegionRegistry registry;
	MockChunkManager manager;
	
	size_t size = sizeof(gblBuffer);
	registry.setEntry(gblBuffer,size,&manager);

	EXPECT_EQ(NULL,(void*)registry.getSegment(gblBuffer-1));
	EXPECT_FALSE(registry.hasEntry(gblBuffer+size));

	ASSERT_EQ(gblBuffer,(void*)registry.getSegment(gblBuffer+size - 1));
	ASSERT_TRUE(registry.hasEntry(gblBuffer+size - 1));
	
	for (Size i = 0 ; i < size ; i+=128)
	{
		ASSERT_EQ(gblBuffer,(void*)registry.getSegment(gblBuffer+i));
		ASSERT_TRUE(registry.hasEntry(gblBuffer+i));
	}
}

/*******************  FUNCTION  *********************/
TEST(RegionRegistry,seq4)
{
	RegionRegistry registry;
	MockChunkManager manager;
	
	size_t size = sizeof(gblBuffer) / 2;
	char * seg1 = gblBuffer;
	char * seg2 = gblBuffer + size;
	
	registry.setEntry(seg1,size,&manager);
	registry.setEntry(seg2,size,&manager);

	EXPECT_EQ(NULL,(void*)registry.getSegment(seg1-1));
	EXPECT_TRUE(registry.hasEntry(seg1+size));
	EXPECT_FALSE(registry.hasEntry(seg2+size));
	
	for (Size i = 0 ; i < size ; i+=128)
	{
		ASSERT_EQ(seg1,(void*)registry.getSegment(seg1+i));
		ASSERT_TRUE(registry.hasEntry(seg1+i));
		
		ASSERT_EQ(seg2,(void*)registry.getSegment(seg2+i));
		ASSERT_TRUE(registry.hasEntry(seg2+i));
	}
}

/*******************  FUNCTION  *********************/
TEST(RegionRegistry,seq5)
{
	RegionRegistry registry;
	MockChunkManager manager;
	
	size_t size = sizeof(gblBuffer) / 2;
	char * seg1 = gblBuffer;
	char * seg2 = gblBuffer + size;
	
	registry.setEntry(seg1,size,&manager);
	registry.setEntry(seg2,size,&manager);

	EXPECT_EQ(NULL,(void*)registry.getSegment(seg1-1));
	EXPECT_TRUE(registry.hasEntry(seg1+size));
	EXPECT_FALSE(registry.hasEntry(seg2+size));
	EXPECT_EQ(seg1,(void*)registry.getSegment(seg1));
	EXPECT_TRUE(registry.hasEntry(seg1));
	EXPECT_EQ(seg2,(void*)registry.getSegment(seg2));
	EXPECT_TRUE(registry.hasEntry(seg2));
	
	registry.remove(seg2);
	
	EXPECT_EQ(NULL,(void*)registry.getSegment(seg1-1));
	EXPECT_FALSE(registry.hasEntry(seg1+size));
	EXPECT_FALSE(registry.hasEntry(seg2+size));
	
	for (Size i = 0 ; i < size ; i+=128)
	{
		ASSERT_EQ(seg1,(void*)registry.getSegment(seg1+i));
		ASSERT_TRUE(registry.hasEntry(seg1+i));
		
		ASSERT_EQ(NULL,(void*)registry.getSegment(seg2+i));
		ASSERT_FALSE(registry.hasEntry(seg2+i));
	}
}
