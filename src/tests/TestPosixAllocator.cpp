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

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator, memalign_bug_1)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();
	
	void * ptr1 = alloc.malloc(256);
	void * ptr2 = alloc.memalign(256,240);
	void * ptr3 = alloc.malloc(260);
	
	EXPECT_LT(addrOffset(ptr1,256),ptr2);
	EXPECT_LT(addrOffset(ptr2,256),ptr3);
}

/*******************  FUNCTION  *********************/
TEST(TestPosixAllocator, buggy_seq_1)
{
	PosixAllocator alloc;
	alloc.resetTLSForTest();
	
	std::map<Addr,void*> ptrs;
	
	ptrs[0x1bf4ba0] = NULL;
	ptrs[0x1bf5370] = NULL;
	ptrs[0x1c04d50] = NULL;
	ptrs[0x1c07ad0] = NULL;
	ptrs[0x1bf7340] = alloc.realloc(ptrs[0x1bf4ba0],2056); //timestamp = 42868089826640, thread = 1
	ptrs[0x1bf89d0] = alloc.malloc(1080); //timestamp = 42868093338560, thread = 1
	ptrs[0x1bf9490] = alloc.realloc(ptrs[0x1bf7340],4112); //timestamp = 42868095797644, thread = 1
	ptrs[0x1bfa4b0] = alloc.realloc(ptrs[0x1bf5370],2242); //timestamp = 42868095964422, thread = 1
	ptrs[0x1bf9490] = alloc.realloc(ptrs[0x1bf9490],8); //timestamp = 42868095992548, thread = 1
	ptrs[0x1bfaf20] = alloc.malloc(256); //timestamp = 42868103998300, thread = 1
	ptrs[0x1bfb030] = alloc.malloc(256); //timestamp = 42868104075980, thread = 1
	ptrs[0x1bfba00] = alloc.malloc(256); //timestamp = 42868105696022, thread = 1
	ptrs[0x1bfd400] = alloc.malloc(216); //timestamp = 42868128278838, thread = 1
	ptrs[0x1bfd730] = alloc.calloc(1,1040); //timestamp = 42868130690552, thread = 1
	ptrs[0x1bfdb50] = alloc.calloc(1177,1); //timestamp = 42868131294146, thread = 1
	ptrs[0x1bfe220] = alloc.malloc(136); //timestamp = 42868133172960, thread = 1
	ptrs[0x1bfe440] = alloc.malloc(152); //timestamp = 42868133495510, thread = 1
	ptrs[0x1bfe4e0] = alloc.malloc(320); //timestamp = 42868133971330, thread = 1
	ptrs[0x1bff660] = alloc.malloc(544); //timestamp = 42868134129966, thread = 1
	ptrs[0x1bff340] = alloc.malloc(376); //timestamp = 42868134714562, thread = 1
	ptrs[0x1c01fa0] = alloc.malloc(320); //timestamp = 42868135372538, thread = 1
	ptrs[0x1c020f0] = alloc.malloc(3660); //timestamp = 42868135399442, thread = 1
	ptrs[0x1c03120] = alloc.malloc(3072); //timestamp = 42868135483438, thread = 1
	ptrs[0x1c03d30] = alloc.malloc(216); //timestamp = 42868141731602, thread = 1
	ptrs[0x1c04930] = alloc.malloc(512); //timestamp = 42868142378520, thread = 1
	ptrs[0x1c04b40] = alloc.malloc(512); //timestamp = 42868142394874, thread = 1
	ptrs[0x1c05040] = alloc.malloc(512); //timestamp = 42868143520274, thread = 1
	ptrs[0x1c05270] = alloc.realloc(ptrs[0x1c04d50],496); //timestamp = 42868143552616, thread = 1
	alloc.free(ptrs[0x1c05040]); //timestamp = 42868143615498, thread = 1
	alloc.free(ptrs[0x1c04930]); //timestamp = 42868143695602, thread = 1
	ptrs[0x1c051b0] = alloc.malloc(136); //timestamp = 42868145333188, thread = 1
	alloc.free(ptrs[0x1c03d30]); //timestamp = 42868145382512, thread = 1
	ptrs[0x1c05750] = alloc.malloc(512); //timestamp = 42868145811116, thread = 1
	ptrs[0x1c05d80] = alloc.malloc(512); //timestamp = 42868147187810, thread = 1
	ptrs[0x1c061d0] = alloc.malloc(512); //timestamp = 42868147204130, thread = 1
	ptrs[0x1c05750] = alloc.malloc(304); //timestamp = 42868152969306, thread = 1
	ptrs[0x1c05830] = alloc.malloc(192); //timestamp = 42868161651692, thread = 1
	ptrs[0x1c07040] = alloc.malloc(512); //timestamp = 42868161668420, thread = 1
	ptrs[0x1c07250] = alloc.malloc(512); //timestamp = 42868161686482, thread = 1
	ptrs[0x1c07460] = alloc.malloc(512); //timestamp = 42868161751396, thread = 1
	ptrs[0x1c07880] = alloc.malloc(512); //timestamp = 42868161961602, thread = 1
	void * ptr1 = ptrs[0x1c07880];
	Size s1 = alloc.getInnerSize(ptr1);
	ptrs[0x1c07b50] = alloc.malloc(512); //timestamp = 42868162071430, thread = 1
	ptrs[0x1c07d60] = alloc.malloc(512); //timestamp = 42868162088702, thread = 1
	ptrs[0x1c07f70] = alloc.realloc(ptrs[0x1c07ad0],240); //timestamp = 42868162120058, thread = 1
	alloc.free(ptrs[0x1c07d60]); //timestamp = 42868162201318, thread = 1
	alloc.free(ptrs[0x1c07b50]); //timestamp = 42868162313544, thread = 1
	alloc.free(ptrs[0x1c07250]); //timestamp = 42868163096802, thread = 1
	alloc.free(ptrs[0x1c07460]); //timestamp = 42868163192810, thread = 1
	alloc.free(ptrs[0x1c07040]); //timestamp = 42868163237112, thread = 1
	ptrs[0x1c071f0] = alloc.malloc(392); //timestamp = 42868164957342, thread = 1
	ptrs[0x1c07530] = alloc.calloc(1,252); //timestamp = 42868165456070, thread = 1
	ptrs[0x1c08100] = alloc.calloc(1,504); //timestamp = 42868165474252, thread = 1
	ptrs[0x1c08300] = alloc.calloc(1,504); //timestamp = 42868165490954, thread = 1
	ptrs[0x1c08d00] = alloc.memalign(256,240); //timestamp = 42868165591092, thread = 1
	void * ptr2 = ptrs[0x1c08d00];
	Size s2 = alloc.getInnerSize(ptr2);

	EXPECT_EQ(true,checkNoOverlap(ptr1,s1,ptr2,s2));
}
