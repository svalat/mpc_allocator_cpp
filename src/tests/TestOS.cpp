/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <OS.h>
#include <gtest/gtest.h>
#include <cstdlib>

/**********************  USING  *********************/
using namespace MPCAllocator;

#define SIZE (2*1024*1024)

TEST(TestOS,mmapAndMunmap)
{
	char * ptr = (char*)OS::mmap(NULL,SIZE);
	ASSERT_NE((char*)NULL,ptr);
	for (int i = 0 ; i < SIZE ; i++)
		ASSERT_EQ(0,ptr[i]);
	memset(ptr,0,SIZE);
	OS::munmap(ptr,SIZE);
}

TEST(TestOS,mremap)
{
	char * ptr = (char*)OS::mmap(NULL,SIZE);
	ASSERT_NE((char*)NULL,ptr);
	for (int i = 0 ; i < SIZE ; i++)
	{
		ASSERT_EQ(0,ptr[i]);
		ptr[i] = i;
	}
	
	ptr = (char*)OS::mremap(ptr,SIZE,2*SIZE);
	for (int i = 0 ; i < SIZE ; i++)
		ASSERT_EQ((char)i,ptr[i]);
	for (int i = SIZE ; i < 2*SIZE ; i++)
		ASSERT_EQ(0,ptr[i]);
	memset(ptr,0,2*SIZE);
}