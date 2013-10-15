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
	alloc.resetTLSForTest();
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,malloc1)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();

	void * ptr1 = alloc.malloc(16);
	EXPECT_NE((void*)NULL,ptr1);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,malloc2)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();

	void * ptr1 = alloc.malloc(0);
	EXPECT_NE((void*)NULL,ptr1);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,free1)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();

	void * ptr1 = alloc.malloc(16);
	EXPECT_NE((void*)NULL,ptr1);
	alloc.free(ptr1);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,free2)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();

	alloc.free(NULL);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,calloc_small)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();

	void * ptr = alloc.malloc(16);
	memset(ptr,1,16);
	alloc.free(ptr);
	unsigned char * ptr2 = (unsigned char*)alloc.calloc(1,16);
	for (int i = 0 ; i < 16 ; i++)
		ASSERT_EQ(0,(int)ptr2[i]);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator,calloc_big)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();

	void * ptr = alloc.malloc(2*1024*1024);
	memset(ptr,1,2*1024*1024);
	alloc.free(ptr);
	unsigned char * ptr2 = (unsigned char*)alloc.calloc(1,2*1024*1024);
	for (int i = 0 ; i < 2*1024*1024 ; i++)
		ASSERT_EQ(0,(int)ptr2[i]);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator, malloc_all_small_sizes)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();

	for (int i = 0 ; i <= 128 ; i++)
		alloc.malloc(i);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator, malloc_all_medium_sizes)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();

	for (int i = 256 ; i <= 2*1024*1024 ; i*=2)
		alloc.malloc(i);
}
