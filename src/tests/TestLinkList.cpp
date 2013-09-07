/********************  HEADERS  *********************/
#include "DoubleLinkList.h"
#include "gtest/gtest.h"

/**********************  USING  *********************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, constructor) {
	DoubleLinkList<int> list;
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, isEmpty) {
	ListElement data;
	DoubleLinkList<ListElement> list;
	
	ASSERT_TRUE(list.isEmpty());
	list.putFirst(&data);
	ASSERT_FALSE(list.isEmpty());
	list.popFirst();
	ASSERT_TRUE(list.isEmpty());
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, putFirst) {
	ListElement data1;
	ListElement data2;
	DoubleLinkList<ListElement> list;
	
	list.putFirst(&data1);
	list.putFirst(&data2);
	
	ListElement * data = list.popFirst();
	ASSERT_EQ(&data2,data);
	data = list.popFirst();
	ASSERT_EQ(&data1,data);
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, putLast) {
	ListElement data1;
	ListElement data2;
	DoubleLinkList<ListElement> list;
	
	list.putLast(&data1);
	list.putLast(&data2);
	
	ListElement * data = list.popLast();
	ASSERT_EQ(&data2,data);
	data = list.popLast();
	ASSERT_EQ(&data1,data);
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, popFirst) {
	ListElement data1;
	ListElement data2;
	ListElement data3;
	DoubleLinkList<ListElement> list;
	
	list.putLast(&data1);
	list.putLast(&data2);
	list.putLast(&data3);
	
	ListElement * data = list.popFirst();
	ASSERT_EQ(&data1,data);
	data = list.popFirst();
	ASSERT_EQ(&data2,data);
	data = list.popFirst();
	ASSERT_EQ(&data3,data);
	data = list.popFirst();
	ASSERT_EQ(NULL,data);
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, popLast) {
	ListElement data1;
	ListElement data2;
	ListElement data3;
	DoubleLinkList<ListElement> list;
	
	list.putLast(&data1);
	list.putLast(&data2);
	list.putLast(&data3);
	
	ListElement * data = list.popLast();
	ASSERT_EQ(&data3,data);
	data = list.popLast();
	ASSERT_EQ(&data2,data);
	data = list.popLast();
	ASSERT_EQ(&data1,data);
	data = list.popLast();
	ASSERT_EQ(NULL,data);
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, remove_1) {
	ListElement data1;
	ListElement data2;
	ListElement data3;
	DoubleLinkList<ListElement> list;
	
	list.putLast(&data1);
	list.putLast(&data2);
	list.putLast(&data3);
	DoubleLinkList<ListElement> * res = list.remove(&data2);
	ASSERT_EQ(NULL,res);
	
	ListElement * data = list.popFirst();
	ASSERT_EQ(&data1,data);
	data = list.popFirst();
	ASSERT_EQ(&data3,data);
	data = list.popFirst();
	ASSERT_EQ(NULL,data);
	ASSERT_TRUE(list.isEmpty());
}

/*******************  FUNCTION  *********************/
TEST(DoubleLinkList, remove_2) {
	ListElement data1;
	DoubleLinkList<ListElement> list;
	
	list.putLast(&data1);
	DoubleLinkList<ListElement> * res = list.remove(&data1);
	ASSERT_EQ(&list,res);
	
	ListElement * data = list.popFirst();
	ASSERT_EQ(NULL,data);
	ASSERT_TRUE(list.isEmpty());
}