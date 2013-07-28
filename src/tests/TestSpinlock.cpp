/********************  HEADERS  *********************/
#include "Spinlock.h"
#include "gtest/gtest.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#else
#define omp_get_num_threads() 1
#endif//HAVE_OPENMP

/*********************  CONSTS  *********************/
#define REPEAT 500000

/*******************  FUNCTION  *********************/
TEST(TestSpinlock, constructor) {
	Spinlock lock;
}

/*******************  FUNCTION  *********************/
TEST(TestSpinlock, correctness) {
	Spinlock lock;
	int sum = 0;
	int nbthread = 1;
	
	#pragma omp parallel
	{
		#pragma omp single
		nbthread = omp_get_num_threads();
		
		lock.lock();
		for (int i = 0 ; i < REPEAT ; i++)
			sum ++;
		lock.unlock();
	}
	
	EXPECT_EQ(nbthread*REPEAT,sum);
}

/*******************  FUNCTION  *********************/
TEST(TestSpinlock, correctnessOfARIIMode_1) {
	Spinlock lock;
	int sum = 0;
	int nbthread = 1;
	
	#pragma omp parallel
	{
		#pragma omp single
		nbthread = omp_get_num_threads();
		
		START_CRITICAL(lock)
			for (int i = 0 ; i < REPEAT ; i++)
				sum ++;
		END_CRITICAL
	}
	
	EXPECT_EQ(nbthread*REPEAT,sum);
}

/*******************  FUNCTION  *********************/
TEST(TestSpinlock, correctnessOfARIIMode_2) {
	Spinlock lock;
	int sum = 0;
	int nbthread = 1;
	
	#pragma omp parallel
	{
		#pragma omp single
		nbthread = omp_get_num_threads();
		
		OPTINAL_CRITICAL(lock,true)
			for (int i = 0 ; i < REPEAT ; i++)
				sum ++;
		END_CRITICAL
	}
	
	EXPECT_EQ(nbthread*REPEAT,sum);
}

/*******************  FUNCTION  *********************/
TEST(TestSpinlock, correctnessOfARIIMode_3) {
	Spinlock lock;
	int sum = 0;
	int nbthread = 1;
	
	#pragma omp parallel
	{
		#pragma omp single
		nbthread = omp_get_num_threads();
		
		//already lock to check optional false (if true it will deadlock)
		lock.lock();
		
		OPTINAL_CRITICAL(lock,false)
			for (int i = 0 ; i < REPEAT ; i++)
				sum ++;
		END_CRITICAL
		
		lock.unlock();
	}
	
	EXPECT_EQ(nbthread*REPEAT,sum);
}
