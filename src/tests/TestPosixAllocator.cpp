/********************  HEADERS  *********************/
#include <PosixAllocator.h>
#include <gtest/gtest.h>

/**********************  USING  *********************/
using namespace testing;
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,setup)
{
	PosixAllocator alloc;
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,malloc1)
{
	PosixAllocator alloc;
	void * ptr1 = alloc.malloc(16);
	EXPECT_NE((void*)NULL,ptr1);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,malloc2)
{
	PosixAllocator alloc;
	void * ptr1 = alloc.malloc(0);
	EXPECT_NE((void*)NULL,ptr1);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,free1)
{
	PosixAllocator alloc;
	void * ptr1 = alloc.malloc(16);
	EXPECT_NE((void*)NULL,ptr1);
	alloc.free(ptr1);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,free2)
{
	PosixAllocator alloc;
	alloc.free(NULL);
}