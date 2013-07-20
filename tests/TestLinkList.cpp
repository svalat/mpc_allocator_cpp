/********************  HEADERS  *********************/
#include "../DoubleLinkList.h"
#include "gtest/gtest.h"

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, constructor) {
	DoubleLinkList<int> list;
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, isEmpty) {
	int data[32] = {0};
	DoubleLinkList<int> list;
	
	ASSERT_TRUE(list.isEmpty());
	list.putFirst(data);
	ASSERT_FALSE(list.isEmpty());
	list.popFirst();
	ASSERT_TRUE(list.isEmpty());
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, putFirst) {
	int data1[32] = {1};
	int data2[32] = {2};
	DoubleLinkList<int> list;
	
	list.putFirst(data1);
	list.putFirst(data2);
	
	int * data = list.popFirst();
	ASSERT_EQ(data2,data);
	data = list.popFirst();
	ASSERT_EQ(data1,data);
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, putLast) {
	int data1[32] = {1};
	int data2[32] = {2};
	DoubleLinkList<int> list;
	
	list.putLast(data1);
	list.putLast(data2);
	
	int * data = list.popLast();
	ASSERT_EQ(data2,data);
	data = list.popLast();
	ASSERT_EQ(data1,data);
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, popFirst) {
	int data1[32] = {1};
	int data2[32] = {2};
	int data3[32] = {3};
	DoubleLinkList<int> list;
	
	list.putLast(data1);
	list.putLast(data2);
	list.putLast(data3);
	
	int * data = list.popFirst();
	ASSERT_EQ(data1,data);
	data = list.popFirst();
	ASSERT_EQ(data2,data);
	data = list.popFirst();
	ASSERT_EQ(data3,data);
	data = list.popFirst();
	ASSERT_EQ(NULL,data);
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, popLast) {
	int data1[32] = {1};
	int data2[32] = {2};
	int data3[32] = {3};
	DoubleLinkList<int> list;
	
	list.putLast(data1);
	list.putLast(data2);
	list.putLast(data3);
	
	int * data = list.popLast();
	ASSERT_EQ(data3,data);
	data = list.popLast();
	ASSERT_EQ(data2,data);
	data = list.popLast();
	ASSERT_EQ(data1,data);
	data = list.popLast();
	ASSERT_EQ(NULL,data);
}