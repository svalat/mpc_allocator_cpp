/********************  HEADERS  *********************/
#include <OS.h>
#include <gtest/gtest.h>
#include <cstdlib>

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
