/********************  HEADERS  *********************/
#include "../Chunk.h"
#include "gtest/gtest.h"

/*******************  FUNCTION  *********************/
TEST(Chunk, constructor_1) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,1024);
	
	EXPECT_EQ(buffer,(void*)chunk);
	EXPECT_EQ(1024-sizeof(Chunk),chunk->getTotalSize());
	EXPECT_EQ(NULL,chunk->getPrev());
	chunk->check();
	chunk->getNext()->check();
}


/*******************  FUNCTION  *********************/
TEST(Chunk, constructor_2) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,NULL,512);
	Chunk * chunk2 = Chunk::setup(chunk->getNext(),chunk,512);
	
	EXPECT_EQ(buffer,(void*)chunk);
	EXPECT_EQ(512,chunk->getTotalSize());
	EXPECT_EQ(buffer+512,(void*)chunk->getNext());
	EXPECT_EQ(NULL,chunk->getPrev());
	chunk->check();
}

/*******************  FUNCTION  *********************/
TEST(Chunk, getTotalSize) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,1024);
	
	EXPECT_EQ(buffer,(void*)chunk);
	EXPECT_EQ(1024-sizeof(Chunk),chunk->getTotalSize());
	EXPECT_EQ(sizeof(Chunk),chunk->getNext()->getTotalSize());
}

/*******************  FUNCTION  *********************/
TEST(Chunk, getInnerSize) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,1024);
	
	EXPECT_EQ(buffer,(void*)chunk);
	EXPECT_EQ(1024-2*sizeof(Chunk),chunk->getInnerSize());
	EXPECT_EQ(0,chunk->getNext()->getInnerSize());
}

/*******************  FUNCTION  *********************/
TEST(Chunk, getNext) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,1024);
	Chunk * next = chunk->getNext();
	
	EXPECT_EQ(buffer+1024-sizeof(Chunk),(void*)next);
	EXPECT_EQ(NULL,next->getNext());
}

/*******************  FUNCTION  *********************/
TEST(Chunk, getPrev) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,1024);
	Chunk * next = chunk->getNext();
	
	EXPECT_EQ(chunk,next->getPrev());
	EXPECT_EQ(NULL,chunk->getPrev());
}

/*******************  FUNCTION  *********************/
TEST(Chunk, getStatus) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,1024);
	
	EXPECT_EQ(CHUNK_ALLOCATED,chunk->getStatus());
}

/*******************  FUNCTION  *********************/
TEST(Chunk, setStatus) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,1024);
	
	EXPECT_EQ(CHUNK_ALLOCATED,chunk->getStatus());
	chunk->setStatus(CHUNK_FREE);
	EXPECT_EQ(CHUNK_FREE,chunk->getStatus());
	chunk->check();
}

/*******************  FUNCTION  *********************/
TEST(Chunk, split) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,1024);
	Chunk * last = chunk->getNext();
	
	Chunk * res = chunk->split(256);
	EXPECT_EQ(256,chunk->getInnerSize());
	EXPECT_EQ(256+sizeof(Chunk),chunk->getTotalSize());
	EXPECT_EQ(1024,chunk->getTotalSize() + res->getTotalSize() + last->getTotalSize());
	res->check();
	
	EXPECT_EQ(NULL,chunk->getPrev());
	EXPECT_EQ(res,chunk->getNext());
	EXPECT_EQ(chunk,res->getPrev());
	EXPECT_EQ(last,res->getNext());
	EXPECT_EQ(res,last->getPrev());
}

/*******************  FUNCTION  *********************/
TEST(Chunk, isSingle) {
	char buffer[1024];
	Chunk * chunk = Chunk::setup(buffer,1024);
	Chunk * last = chunk->getNext();
	
	EXPECT_FALSE(chunk->isSingle());
	chunk->setStatus(CHUNK_FREE);
	EXPECT_TRUE(chunk->isSingle());
	
	Chunk * res = chunk->split(256);
	
	EXPECT_FALSE(chunk->isSingle());
}
