/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include "MediumFreePool.h"
#include "gtest/gtest.h"

/**********************  USING  *********************/
using namespace MPCAllocator;

static const Size TEST_SIZE_LIST[NB_FREE_LIST] = {8,16,32,64,128,1,-1lu,-1lu,-1lu,-1lu,-1lu,
	-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,
	-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu,-1lu};

TEST(TestMediumFreePool, constructor_1) {
	MediumFreePool pool;
}

TEST(TestMediumFreePool, constructor_2) {
	MediumFreePool pool(TEST_SIZE_LIST,NULL);
}

TEST(TestMediumFreePool, findChunk_1) {
	MediumFreePool pool(TEST_SIZE_LIST,NULL);
	
	EXPECT_EQ(NULL,pool.findChunk(16));
}

TEST(TestMediumFreePool, findChunk_2) {
	MediumFreePool pool;
	
	EXPECT_EQ(NULL,pool.findChunk(32));
}

TEST(TestMediumFreePool, findChunk_3) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(64);
	MediumChunk * c2 = c1->split(32);
	MediumChunk * c3 = c2->split(64);
	c3->split(64);
	
	pool.insert(c0,CHUNK_INSERT_LIFO);
	pool.insert(c1,CHUNK_INSERT_LIFO);
	pool.insert(c2,CHUNK_INSERT_LIFO);
	pool.insert(c3,CHUNK_INSERT_LIFO);

	EXPECT_EQ(c1,pool.findChunk(32));
	EXPECT_EQ(c0,pool.findChunk(64));
	EXPECT_EQ(c2,pool.findChunk(64));
}

TEST(TestMediumFreePool, findChunk_4) {
	char buffer[1024];
	MediumFreePool pool;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(64);
	MediumChunk * c2 = c1->split(32);
	MediumChunk * c3 = c2->split(64);
	c3->split(64);
	
	pool.insert(c0,CHUNK_INSERT_LIFO);
	pool.insert(c1,CHUNK_INSERT_LIFO);
	pool.insert(c2,CHUNK_INSERT_LIFO);
	pool.insert(c3,CHUNK_INSERT_LIFO);

	EXPECT_EQ(c1,pool.findChunk(32));
	EXPECT_EQ(c0,pool.findChunk(64));
	EXPECT_EQ(c2,pool.findChunk(64));
}


TEST(TestMediumFreePool, insert_1) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	
	pool.insert(buffer,64,CHUNK_INSERT_FIFO);
	EXPECT_EQ(NULL,pool.findChunk(24));
}

TEST(TestMediumFreePool, insert_2) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	
	pool.insert(buffer,64,CHUNK_INSERT_FIFO);
	EXPECT_EQ(buffer,(char*)pool.findChunk(16));
	EXPECT_EQ(NULL,pool.findChunk(16));
}

TEST(TestMediumFreePool, insert_3) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(16);
	MediumChunk * c2 = c1->split(16);
	c2->split(16);
	
	pool.insert(c0,CHUNK_INSERT_LIFO);
	pool.insert(c1,CHUNK_INSERT_LIFO);
	pool.insert(c2,CHUNK_INSERT_LIFO);
	EXPECT_EQ(c0,pool.findChunk(16));
	EXPECT_EQ(c1,pool.findChunk(16));
	EXPECT_EQ(c2,pool.findChunk(16));
}

TEST(TestMediumFreePool, insert_4) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(16);
	MediumChunk * c2 = c1->split(16);
	c2->split(16);
	
	pool.insert(c0,CHUNK_INSERT_FIFO);
	pool.insert(c1,CHUNK_INSERT_FIFO);
	pool.insert(c2,CHUNK_INSERT_FIFO);
	EXPECT_EQ(c2,pool.findChunk(16));
	EXPECT_EQ(c1,pool.findChunk(16));
	EXPECT_EQ(c0,pool.findChunk(16));
}

TEST(TestMediumFreePool, insert_5) {
	char buffer[1024];
	MediumFreePool pool;;
	
	pool.insert(buffer,72,CHUNK_INSERT_FIFO);
	EXPECT_EQ(NULL,pool.findChunk(32));
}


TEST(TestMediumFreePool, remove_1) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(16);
	MediumChunk * c2 = c1->split(16);
	
	pool.insert(c0,CHUNK_INSERT_LIFO);
	pool.insert(c1,CHUNK_INSERT_LIFO);
	pool.insert(c2,CHUNK_INSERT_LIFO);
	pool.remove(c1);
	EXPECT_EQ(c0,pool.findChunk(16));
	EXPECT_EQ(c2,pool.findChunk(16));
}

TEST(TestMediumFreePool, merge_1) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(16);
	MediumChunk * c2 = c1->split(16);
	
	pool.insert(c0,CHUNK_INSERT_LIFO);
	pool.insert(c2,CHUNK_INSERT_LIFO);

	MediumChunk * c = pool.merge(c1);
	
	EXPECT_EQ(c0,c);
	EXPECT_EQ(NULL,pool.findChunk(16));
}

TEST(TestMediumFreePool, merge_2) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(16);
	c1->split(16);

	MediumChunk * c = pool.merge(c1);
	
	EXPECT_EQ(c1,c);
}

TEST(TestMediumFreePool, merge_3) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(16);
	c1->split(16);
	
	//pool.insert(c1,CHUNK_INSERT_LIFO);
// 	pool.insert(c2,CHUNK_INSERT_LIFO);

	MediumChunk * res = pool.tryMergeForSize(c0,34);
	EXPECT_EQ(NULL,res);
}

TEST(TestMediumFreePool, merge_4) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(16);
	MediumChunk * c2 = c1->split(16);
	
	pool.insert(c1,CHUNK_INSERT_LIFO);
	pool.insert(c2,CHUNK_INSERT_LIFO);

	MediumChunk * res = pool.tryMergeForSize(c0,34);
	EXPECT_EQ(c0,res);
	EXPECT_EQ(16*2+sizeof(MediumChunk),res->getInnerSize());
	
	EXPECT_EQ(c2,pool.findChunk(16));
}

TEST(TestMediumFreePool, merge_5) {
	char buffer[1024];
	MediumFreePool pool(TEST_SIZE_LIST,NULL);;
	MediumChunk * c0 = MediumChunk::setup(buffer,1024);
	MediumChunk * c1 = c0->split(16);
	MediumChunk * c2 = c1->split(16);
	c2->split(16);
	
	pool.insert(c1,CHUNK_INSERT_LIFO);
	pool.insert(c2,CHUNK_INSERT_LIFO);

	MediumChunk * res = pool.tryMergeForSize(c0,64);
	EXPECT_EQ(c0,res);
	EXPECT_EQ(16*3+2*sizeof(MediumChunk),res->getInnerSize());
	
	EXPECT_EQ(NULL,pool.findChunk(16));
}

TEST(TestMediumFreePool, analyticFunc) {
	for (int i = 0 ; i < 44 ; i++)
	{
		if (cstDefaultFreeSizes[i] > 0)
		{
			EXPECT_EQ(cstDefaultFreeSizes[i],cstDefaultFreeSizes[reverseDefaultFreeSizes(cstDefaultFreeSizes[i],cstDefaultFreeSizes,45)]);
			EXPECT_EQ(i,reverseDefaultFreeSizes(cstDefaultFreeSizes[i],cstDefaultFreeSizes,45));
			EXPECT_EQ(cstDefaultFreeSizes[i],cstDefaultFreeSizes[reverseDefaultFreeSizes(cstDefaultFreeSizes[i]+1,cstDefaultFreeSizes,45)]);
			EXPECT_EQ(i,reverseDefaultFreeSizes(cstDefaultFreeSizes[i]+1,cstDefaultFreeSizes,45));
		}
	}
	
	EXPECT_EQ(2*1024*1024ul,cstDefaultFreeSizes[44]);
	EXPECT_EQ(-1lu,cstDefaultFreeSizes[45]);
	EXPECT_EQ(44,reverseDefaultFreeSizes(2*1024*1024,cstDefaultFreeSizes,45));
	EXPECT_EQ(45,reverseDefaultFreeSizes(2*1024*1024+1,cstDefaultFreeSizes,45));
	EXPECT_EQ(45,reverseDefaultFreeSizes(20*1024*1024+1,cstDefaultFreeSizes,45));
}
