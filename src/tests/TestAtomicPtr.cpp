/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <AtomicPtr.h>
#include <gtest/gtest.h>

/**********************  USING  *********************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
TEST(TestAtomicPtr,defaultConstructor)
{
	AtomicPtr<void> ptr;
}

/*******************  FUNCTION  *********************/
TEST(TestAtomicPtr,loadSeq)
{
	AtomicPtr<void> ptr(NULL);
	EXPECT_EQ((void*)NULL,ptr.atomicLoad());
}

/*******************  FUNCTION  *********************/
TEST(TestAtomicPtr,storeSeq)
{
	AtomicPtr<void> ptr;
	ptr.atomicStore((void*)0x42);
	EXPECT_EQ((void*)0x42,ptr.atomicLoad());
}


/*******************  FUNCTION  *********************/
TEST(TestAtomicPtr,swapSeq)
{
	AtomicPtr<void> ptr(NULL);
	EXPECT_EQ((void*)NULL,ptr.atomicSwap((void*)0x42));
	EXPECT_EQ((void*)0x42,ptr.atomicSwap((void*)0x43));
	EXPECT_EQ((void*)0x43,ptr.atomicSwap((void*)0x44));
}
