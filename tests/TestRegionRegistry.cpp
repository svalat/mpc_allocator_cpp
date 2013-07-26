/********************  HEADERS  *********************/
#include <RegionRegistry.h>
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

#warning TODO
