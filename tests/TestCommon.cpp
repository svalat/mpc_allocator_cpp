/********************  HEADERS  *********************/
#include "Common.h"
#include "gtest/gtest.h"

TEST(TestCommon,ceilToPowOf2)
{
	EXPECT_EQ(8,ceilToPowOf2(8,8));
	EXPECT_EQ(8,ceilToPowOf2(9,8));
	EXPECT_EQ(8,ceilToPowOf2(15,8));
	EXPECT_EQ(16,ceilToPowOf2(16,8));
	EXPECT_EQ(0,ceilToPowOf2(8,16));
}

TEST(TestCommon,upToPowOf2)
{
	EXPECT_EQ(8,upToPowOf2(8,8));
	EXPECT_EQ(16,upToPowOf2(9,8));
	EXPECT_EQ(16,upToPowOf2(15,8));
	EXPECT_EQ(16,upToPowOf2(16,8));
	EXPECT_EQ(16,upToPowOf2(8,16));
}

TEST(TestCommon,addrOffset)
{
	char tmp[128];
	uint64_t * s = (uint64_t*)tmp;
	EXPECT_EQ((uint64_t*)(tmp+1),addrOffset(s,1));
}

TEST(TestCommon,max)
{
	EXPECT_EQ(10,max(1,10));
	EXPECT_EQ(10,max(10,1));
	EXPECT_EQ(10,max(10,10));
}

TEST(TestCommon,fastLog2)
{
	for (int i = 0 ; i < 32 ; i++)
	{
		EXPECT_EQ(i,fastLog2(1UL<<i));
		if (i > 0)
			EXPECT_EQ(i,fastLog2((1UL<<i)+1));
	}
}