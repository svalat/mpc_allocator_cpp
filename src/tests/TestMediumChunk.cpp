/********************  HEADERS  *********************/
#include "MediumChunk.h"
#include "gtest/gtest.h"

/**********************  USING  *********************/
using namespace MPCAllocator;

/*********************  CLASS  **********************/
class TestMediumChunk : public ::testing::Test {
	protected:
		void SetUp(void)
		{
			memset(buffer,0,1024);
			chunk = MediumChunk::setup(buffer,1024);
		}
		char buffer[1024];
		MediumChunk * chunk;
};

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, typeSize) {
	EXPECT_EQ(sizeof(size_t),sizeof(Size));
	EXPECT_EQ(sizeof(void*),sizeof(Addr));
	EXPECT_EQ(3 * sizeof(void*),sizeof(MediumChunk));
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, constructor_1) {	
	EXPECT_EQ(buffer,(void*)chunk);
	EXPECT_EQ(1024-sizeof(MediumChunk),chunk->getTotalSize());
	EXPECT_EQ(NULL,chunk->getPrev());
	chunk->check();
	chunk->getNext()->check();
}


/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, constructor_2) {
	MediumChunk * chunk = MediumChunk::setup(buffer,NULL,512);
	MediumChunk::setup(chunk->getNext(),chunk,512);
	
	EXPECT_EQ(buffer,(void*)chunk);
	EXPECT_EQ(512u,chunk->getTotalSize());
	EXPECT_EQ(buffer+512,(void*)chunk->getNext());
	EXPECT_EQ(NULL,chunk->getPrev());
	chunk->check();
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, getTotalSize) {
	EXPECT_EQ(buffer,(void*)chunk);
	EXPECT_EQ(1024-sizeof(MediumChunk),chunk->getTotalSize());
	EXPECT_EQ(sizeof(MediumChunk),chunk->getNext()->getTotalSize());
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, getInnerSize) {
	EXPECT_EQ(buffer,(void*)chunk);
	EXPECT_EQ(1024-2*sizeof(MediumChunk),chunk->getInnerSize());
	EXPECT_EQ(0u,chunk->getNext()->getInnerSize());
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, getNext) {
	MediumChunk * next = chunk->getNext();
	
	EXPECT_EQ(buffer+1024-sizeof(MediumChunk),(void*)next);
	EXPECT_EQ(NULL,next->getNext());
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, getPrev) {
	MediumChunk * next = chunk->getNext();
	
	EXPECT_EQ(chunk,next->getPrev());
	EXPECT_EQ(NULL,chunk->getPrev());
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, getStatus) {
	EXPECT_EQ(CHUNK_ALLOCATED,chunk->getStatus());
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, setStatus) {
	EXPECT_EQ(CHUNK_ALLOCATED,chunk->getStatus());
	chunk->setStatus(CHUNK_FREE);
	EXPECT_EQ(CHUNK_FREE,chunk->getStatus());
	chunk->check();
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, split_1) {
	MediumChunk * last = chunk->getNext();
	
	MediumChunk * res = chunk->split(256);
	EXPECT_EQ(256u,chunk->getInnerSize());
	EXPECT_EQ(256+sizeof(MediumChunk),chunk->getTotalSize());
	EXPECT_EQ(1024u,chunk->getTotalSize() + res->getTotalSize() + last->getTotalSize());
	res->check();
	
	EXPECT_EQ(NULL,chunk->getPrev());
	EXPECT_EQ(res,chunk->getNext());
	EXPECT_EQ(chunk,res->getPrev());
	EXPECT_EQ(last,res->getNext());
	EXPECT_EQ(res,last->getPrev());
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, split_2) {
	EXPECT_EQ(NULL,chunk->split(2048));
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, isSingle) {
	EXPECT_TRUE(chunk->isSingle());
	chunk->setStatus(CHUNK_FREE);
	EXPECT_TRUE(chunk->isSingle());
	
	chunk->split(256);
	
	EXPECT_FALSE(chunk->isSingle());
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, getPtr) {
	MediumChunk * invalid = NULL;
	
	EXPECT_EQ(NULL,invalid->getPtr());
	EXPECT_EQ(chunk+1,chunk->getPtr());
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, getChunk) {
	EXPECT_EQ(NULL,MediumChunk::getChunk(NULL));
	EXPECT_EQ(chunk,MediumChunk::getChunk(chunk->getPtr()));
}

/*******************  FUNCTION  *********************/
TEST_F(TestMediumChunk, merge) {
	MediumChunk * c2 = chunk->split(128);
	MediumChunk * c3 = c2->split(128);
	MediumChunk * c4 = c3->split(128);
	MediumChunk * c5 = c4->split(128);
	
	c2->merge(c4);
	
	EXPECT_EQ(3*(128+sizeof(MediumChunk)),c2->getTotalSize());
	EXPECT_EQ(c5,c2->getNext());
	EXPECT_EQ(chunk,c2->getPrev());
	EXPECT_EQ(c2,chunk->getNext());
	EXPECT_EQ(c2,c5->getPrev());
}
