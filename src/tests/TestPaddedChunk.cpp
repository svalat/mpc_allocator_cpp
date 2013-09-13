/********************  HEADERS  *********************/
#include <MediumChunk.h>
#include <PaddedChunk.h>
#include <gtest/gtest.h>

/**********************  USING  *********************/
using namespace testing;
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
TEST(TestPaddedChunk,setup1)
{
	char buffer[4096];

	MediumChunk * chunk = MediumChunk::setup(buffer,sizeof(buffer));
	PaddedChunk * padded = PaddedChunk::setup(chunk,16);
	ASSERT_NE((void*)NULL,padded);
	ASSERT_GE((void*)padded,(void*)chunk);
}

/*******************  FUNCTION  *********************/
TEST(TestPaddedChunk,setup2)
{
	char buffer[4096];

	PaddedChunk * padded = PaddedChunk::setup(buffer,16,sizeof(buffer));
	ASSERT_NE((void*)NULL,padded);
	ASSERT_GE((void*)padded,(void*)buffer);
}

/*******************  FUNCTION  *********************/
TEST(TestPaddedChunk,getPtr)
{
	char buffer[4096];

	MediumChunk * chunk = MediumChunk::setup(buffer,sizeof(buffer));
	PaddedChunk * padded = PaddedChunk::setup(chunk,16);
	
	ASSERT_EQ(buffer+sizeof(MediumChunk)+16,padded->getPtr());
}

/*******************  FUNCTION  *********************/
TEST(TestPaddedChunk,pad)
{
	char buffer[4096];
	char * tmp = (char*)PaddedChunk::pad(buffer,16,sizeof(buffer));
	
	ASSERT_EQ(buffer+16,tmp);
}

/*******************  FUNCTION  *********************/
TEST(TestPaddedChunk,unpad_good)
{
	char buffer[4096];
	char * tmp = (char*)PaddedChunk::pad(buffer,16,sizeof(buffer));
	
	ASSERT_EQ(buffer,PaddedChunk::unpad(tmp));
}

/*******************  FUNCTION  *********************/
TEST(TestPaddedChunk,unpad_bad)
{
	char buffer[4096];
	memset(buffer,0,sizeof(buffer));
	
	ASSERT_EQ(buffer+4,PaddedChunk::unpad(buffer+4));
}

/*******************  FUNCTION  *********************/
TEST(TestPaddedChunk,unpad_NULL)
{
	ASSERT_EQ((void*)NULL,PaddedChunk::unpad(NULL));
}
