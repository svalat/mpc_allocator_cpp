/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <Common.h>
#include <DoubleLinkList.h>
#include <SmallChunkRun.h>
#include <gtest/gtest.h>

/**********************  USING  *********************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,classSize)
{
	EXPECT_EQ(SMALL_RUN_SIZE,sizeof(SmallChunkRun));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,constructor_1)
{
	SmallChunkRun run;
	EXPECT_EQ(0,run.getSplitting());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,constructor_2)
{
	SmallChunkRun run(0,16);
	EXPECT_EQ(16,run.getSplitting());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,getSplitting)
{
	SmallChunkRun run(0,16);
	EXPECT_EQ(16,run.getSplitting());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,getInnerSize)
{
	SmallChunkRun run(0,16);
	void * ptr = run.malloc(16);
	EXPECT_EQ(16u,run.getInnerSize(ptr));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,getTotalSize)
{
	SmallChunkRun run(0,16);
	void * ptr = run.malloc(16);
	EXPECT_EQ(16u,run.getTotalSize(ptr));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,getRequestSize)
{
	SmallChunkRun run(0,16);
	void * ptr = run.malloc(16);
	EXPECT_EQ(0u,run.getRequestedSize(ptr));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,constructor_3)
{
	char buffer[SMALL_RUN_SIZE];
	SmallChunkRun * run = new (buffer) SmallChunkRun;
	EXPECT_EQ(buffer,(char*)run);
	EXPECT_EQ(0u,run->getSplitting());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,setSplitting)
{
	SmallChunkRun run;
	EXPECT_EQ(0u,run.getSplitting());
	run.setSplitting(16);
	EXPECT_EQ(16u,run.getSplitting());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,setSplittingNonMultiple)
{
	SmallChunkRun run;
	EXPECT_EQ(0u,run.getSplitting());
	run.setSplitting(15);
	EXPECT_EQ(15u,run.getSplitting());
	while(run.malloc(15) != NULL);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,malloc_1)
{
	SmallChunkRun run(0,16);
	
	void * ptr = run.malloc(16);
	EXPECT_NE((void*)NULL,ptr);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,malloc_2)
{
	SmallChunkRun run(0,16);
	
	void * ptr1 = run.malloc(16);
	ASSERT_NE((void*)NULL,ptr1);
	void * ptr2 = run.malloc(16);
	EXPECT_NE(ptr1,ptr2);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,free)
{
	SmallChunkRun run(0,16);
	
	void * ptr1 = run.malloc(16);
	run.malloc(16);
	run.free(ptr1);
	void * ptr3 = run.malloc(16);
	EXPECT_EQ(ptr1,ptr3);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,full)
{
	SmallChunkRun run(0,16);
	unsigned int cnt = 0;
	
	while ( run.malloc(16) != NULL )
		cnt++;
	
	EXPECT_EQ(SMALL_RUN_SIZE/16 - 4,cnt);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,fullNoOverlap)
{
	unsigned char * ptr[SMALL_RUN_SIZE/16];
	SmallChunkRun run(0,16);
	unsigned int cnt = 0;
	
	while ( (ptr[cnt] = (unsigned char*)run.malloc(16)) != NULL )
		cnt++;
	
	ASSERT_EQ(SMALL_RUN_SIZE/16 - 4,cnt);
	
	for (unsigned int i = 0 ; i < cnt ; i++)
	{
		for (unsigned int j = 0 ; j < 16 ; j++)
		{
			ptr[i][j] = (unsigned char)-1;
		}
		for (unsigned int j = 0 ; j < i ; j++)
			EXPECT_NE(ptr[j],ptr[i]);
	}
		
	EXPECT_EQ(NULL,run.malloc(16));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,freeMiddle)
{
	unsigned char * ptr[SMALL_RUN_SIZE/16];
	SmallChunkRun run(0,16);
	int cnt = 0;
	
	//alloc all
	while ( (ptr[cnt] = (unsigned char*)run.malloc(16)) != NULL );
	
	//free one of them
	run.free(ptr[32]);
	
	//try to realloc it
	EXPECT_EQ(ptr[32],run.malloc(16));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,isEmpty)
{
	SmallChunkRun run(0,16);
	EXPECT_TRUE(run.isEmpty());
	void * ptr = run.malloc(16);
	EXPECT_FALSE(run.isEmpty());
	run.free(ptr);
	EXPECT_TRUE(run.isEmpty());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,isFull)
{
	SmallChunkRun run(0,16);
	EXPECT_FALSE(run.isFull());
	while (run.malloc(16) != NULL);
	EXPECT_TRUE(run.isFull());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,skipedOffset)
{
	SmallChunkRun run(32,16);
	unsigned int cnt = 0;
	void * ptr;
	
	while ( (ptr = run.malloc(16)) != NULL )
	{
		EXPECT_GE(ptr,(char*)&run+32);
		cnt++;
	}
	
	EXPECT_EQ(SMALL_RUN_SIZE/16 - 4 - 2,cnt);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,skipedOffset2)
{
	SmallChunkRun run(31,16);
	unsigned int cnt = 0;
	void * ptr;
	
	while ( (ptr = run.malloc(16)) != NULL )
	{
		EXPECT_GE(ptr,(char*)&run+32);
		cnt++;
	}
	
	EXPECT_EQ(SMALL_RUN_SIZE/16 - 4 - 2,cnt);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,realloc)
{
	SmallChunkRun run(0,16);
	void * ptr  = run.malloc(16);
	void * ptr2 = run.realloc(ptr,15);
	EXPECT_EQ(ptr,ptr2);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,listHandler1)
{
	SmallChunkRun run;
	
	ListElement * elt = run.getListHandler();
	EXPECT_EQ(&run,SmallChunkRun::getFromListHandler(elt));
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,listHandler2)
{
	DoubleLinkList<SmallChunkRun> list;
	SmallChunkRun run1(0,16);
	
	list.putFirst(&run1);
	
	EXPECT_EQ(&run1,list.popLast());
	EXPECT_EQ(NULL,list.popLast());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallChunk,listHandler3)
{
	DoubleLinkList<SmallChunkRun> list;
	SmallChunkRun run1(0,16);
	SmallChunkRun run2(0,16);
	
	list.putFirst(&run1);
	list.putFirst(&run2);
	
	EXPECT_EQ(&run1,list.popLast());
	EXPECT_EQ(&run2,list.popLast());
	EXPECT_EQ(NULL,list.popLast());
}
