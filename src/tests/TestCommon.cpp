/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <Common.h>
#include <gtest/gtest.h>

/**********************  USING  *********************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
TEST(TestCommon,ceilToPowOf2)
{
	EXPECT_EQ(8u,ceilToPowOf2(8,8));
	EXPECT_EQ(8u,ceilToPowOf2(9,8));
	EXPECT_EQ(8u,ceilToPowOf2(15,8));
	EXPECT_EQ(16u,ceilToPowOf2(16,8));
	EXPECT_EQ(0u,ceilToPowOf2(8,16));
}

/*******************  FUNCTION  *********************/
TEST(TestCommon,upToPowOf2)
{
	EXPECT_EQ(8u,upToPowOf2(8,8));
	EXPECT_EQ(16u,upToPowOf2(9,8));
	EXPECT_EQ(16u,upToPowOf2(15,8));
	EXPECT_EQ(16u,upToPowOf2(16,8));
	EXPECT_EQ(16u,upToPowOf2(8,16));
}

/*******************  FUNCTION  *********************/
TEST(TestCommon,addrOffset)
{
	char tmp[128];
	uint64_t * s = (uint64_t*)tmp;
	EXPECT_EQ((uint64_t*)(tmp+1),addrOffset(s,1));
}

/*******************  FUNCTION  *********************/
TEST(TestCommon,max)
{
	EXPECT_EQ(10,max(1,10));
	EXPECT_EQ(10,max(10,1));
	EXPECT_EQ(10,max(10,10));
}

/*******************  FUNCTION  *********************/
TEST(TestCommon,fastLog2)
{
	for (int i = 0 ; i < 32 ; i++)
	{
		EXPECT_EQ(i,fastLog2(1UL<<i));
		if (i > 0)
			EXPECT_EQ(i,fastLog2((1UL<<i)+1));
	}
}
