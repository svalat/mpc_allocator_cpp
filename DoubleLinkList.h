#ifndef DOUBLELINKLIST_H
#define DOUBLELINKLIST_H

/********************  HEADERS  *********************/
#include "Common.h"

/*********************  STRUCT  *********************/
struct ListElement
{
	ListElement();
	ListElement * next;
	ListElement * prev;
};

/*********************  CLASS  **********************/
template <class T>
class DoubleLinkList
{
	public:
		DoubleLinkList();//checked
		bool isEmpty(void) const;//checked
		void putFirst(T * value);//checked
		void putLast(T * value);//checked
		static void checkSize(Size size);
		T * popFirst(void);
		T * popLast(void);
	private:
		DoubleLinkList ( const DoubleLinkList& other );
		DoubleLinkList& operator= ( const DoubleLinkList& other );
	private:
		ListElement root;
};

/********************  HEADERS  *********************/
#include "DoubleLinkList_impl.h"

#endif // DOUBLELINKLIST_H
