/********************  HEADERS  *********************/
#include <Common.h>
#include <SmallChunkRun.h>
#include <gtest/gtest.h>

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,classSize)
{
	EXPECT_EQ(SMALL_RUN_SIZE,sizeof(SmallChunkRun));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,constructor_1)
{
	SmallChunkRun chunk;
	EXPECT_EQ(0,chunk.getSplitting());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,constructor_2)
{
	SmallChunkRun chunk(0,16);
	EXPECT_EQ(16,chunk.getSplitting());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,constructor_3)
{
	char buffer[SMALL_RUN_SIZE];
	SmallChunkRun * chunk = new (buffer) SmallChunkRun;
	EXPECT_EQ(buffer,(char*)chunk);
	EXPECT_EQ(0,chunk->getSplitting());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,setSplitting)
{
	SmallChunkRun chunk;
	EXPECT_EQ(0,chunk.getSplitting());
	chunk.setSplitting(16);
	EXPECT_EQ(16,chunk.getSplitting());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,malloc_1)
{
	SmallChunkRun chunk(0,16);
	
	void * ptr = chunk.malloc(16);
	EXPECT_EQ(addrOffset(&chunk,0),ptr);
	ptr = chunk.malloc(16);
	EXPECT_EQ(addrOffset(&chunk,16),ptr);
}
