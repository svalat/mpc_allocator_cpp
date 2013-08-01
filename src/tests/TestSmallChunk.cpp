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
	EXPECT_NE((void*)NULL,ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,malloc_2)
{
	SmallChunkRun chunk(0,16);
	
	void * ptr1 = chunk.malloc(16);
	ASSERT_NE((void*)NULL,ptr1);
	void * ptr2 = chunk.malloc(16);
	EXPECT_NE(ptr1,ptr2);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,free)
{
	SmallChunkRun chunk(0,16);
	
	void * ptr1 = chunk.malloc(16);
	void * ptr2 = chunk.malloc(16);
	chunk.free(ptr1);
	void * ptr3 = chunk.malloc(16);
	EXPECT_EQ(ptr1,ptr3);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,full)
{
	SmallChunkRun chunk(0,16);
	int cnt = 0;
	
	while ( chunk.malloc(16) != NULL )
		cnt++;
	
	EXPECT_EQ(SMALL_RUN_SIZE/16 - 4,cnt);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,fullNoOverlap)
{
	unsigned char * ptr[SMALL_RUN_SIZE/16];
	SmallChunkRun chunk(0,16);
	int cnt = 0;
	
	while ( (ptr[cnt] = (unsigned char*)chunk.malloc(16)) != NULL )
		cnt++;
	
	ASSERT_EQ(SMALL_RUN_SIZE/16 - 4,cnt);
	
	for (int i = 0 ; i < cnt ; i++)
	{
		for (int j = 0 ; j < 16 ; j++)
		{
			ptr[i][j] = (unsigned char)-1;
		}
		for (int j = 0 ; j < i ; j++)
			EXPECT_NE(ptr[j],ptr[i]);
	}
		
	EXPECT_EQ(NULL,chunk.malloc(16));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,freeMiddle)
{
	unsigned char * ptr[SMALL_RUN_SIZE/16];
	SmallChunkRun chunk(0,16);
	int cnt = 0;
	
	//alloc all
	while ( (ptr[cnt] = (unsigned char*)chunk.malloc(16)) != NULL );
	
	//free one of them
	chunk.free(ptr[32]);
	
	//try to realloc it
	EXPECT_EQ(ptr[32],chunk.malloc(16));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,isEmpty)
{
	SmallChunkRun chunk(0,16);
	EXPECT_TRUE(chunk.isEmpty());
	void * ptr = chunk.malloc(16);
	EXPECT_FALSE(chunk.isEmpty());
	chunk.free(ptr);
	EXPECT_TRUE(chunk.isEmpty());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,skipedOffset)
{
	SmallChunkRun chunk(32,16);
	int cnt = 0;
	void * ptr;
	
	while ( (ptr = chunk.malloc(16)) != NULL )
	{
		EXPECT_GE(ptr,(char*)&chunk+32);
		cnt++;
	}
	
	EXPECT_EQ(SMALL_RUN_SIZE/16 - 4 - 2,cnt);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,skipedOffset2)
{
	SmallChunkRun chunk(31,16);
	int cnt = 0;
	void * ptr;
	
	while ( (ptr = chunk.malloc(16)) != NULL )
	{
		EXPECT_GE(ptr,(char*)&chunk+32);
		cnt++;
	}
	
	EXPECT_EQ(SMALL_RUN_SIZE/16 - 4 - 2,cnt);
}
