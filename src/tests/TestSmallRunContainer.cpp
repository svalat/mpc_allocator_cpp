/********************  HEADERS  *********************/
#include <SmallRunContainer.h>
#include <gtest/gtest.h>

/**********************  USING  *********************/
using namespace testing;
using namespace MPCAllocator;

/********************  GLOBALS  *********************/
static char gblBuffer[REGION_SPLITTING];

/*******************  FUNCTION  *********************/
TEST(TestSmallRunContainer,setup)
{
	SmallRunContainer * container = SmallRunContainer::setup(gblBuffer,sizeof(gblBuffer));
	EXPECT_EQ(gblBuffer,(char*)container);
};

/*******************  FUNCTION  *********************/
TEST(TestSmallRunContainer,isEmpty)
{
	SmallRunContainer * container = SmallRunContainer::setup(gblBuffer,sizeof(gblBuffer));
	EXPECT_EQ(true,container->isEmpty());
	SmallChunkRun * run = container->getEmptyRun();
	EXPECT_NE((void*)NULL,run);
	EXPECT_FALSE(container->isEmpty());
	container->regEmpty(run);
	EXPECT_TRUE(container->isEmpty());
}

/*******************  FUNCTION  *********************/
TEST(TestSmallRunContainer,getEmptyRun1)
{
	SmallRunContainer * container = SmallRunContainer::setup(gblBuffer,sizeof(gblBuffer));
	SmallChunkRun * run = container->getEmptyRun();
	EXPECT_NE((void*)NULL,run);
	SmallChunkRun * run2 = container->getEmptyRun();
	EXPECT_NE(run,run2);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallRunContainer,getEmptyRun2)
{
	SmallRunContainer * container = SmallRunContainer::setup(gblBuffer,sizeof(gblBuffer));
	SmallChunkRun * run;
	int cnt = 0;
	do {
		run = container->getEmptyRun();
		if (run != NULL)
			cnt++;
	} while(run != NULL);
	
	EXPECT_EQ((void*)NULL,run);
	EXPECT_EQ(sizeof(gblBuffer)/SMALL_RUN_SIZE,cnt);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallRunContainer,getEmptyRun3)
{
	SmallRunContainer * container = SmallRunContainer::setup(gblBuffer,sizeof(gblBuffer));
	SmallChunkRun * run = container->getEmptyRun();
	EXPECT_NE((void*)NULL,run);
	run->setSplitting(16);
	void * chunk;
	do {
		chunk = run->malloc(16);
		if (chunk != NULL)
			EXPECT_GE((void*)chunk,(void*)(container+1));
	} while(chunk != NULL);
}

/*******************  FUNCTION  *********************/
TEST(TestSmallRunContainer,regEmpty)
{
	SmallRunContainer * container = SmallRunContainer::setup(gblBuffer,sizeof(gblBuffer));
	SmallChunkRun * run = container->getEmptyRun();
	container->regEmpty(run);
	SmallChunkRun * run2 = container->getEmptyRun();
	EXPECT_EQ(run,run2);
}
