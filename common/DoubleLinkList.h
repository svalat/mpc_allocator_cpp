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
		class Iterator
		{
			public:
				Iterator(ListElement * elt) {this->cur = elt;};
				Iterator & operator=(const Iterator & it) {this->cur = it.cur;};
				Iterator & operator++(void) {this->cur = this->cur->next; return *this;};
				Iterator & operator--(void) {this->cur = this->cur->prev; return *this;};
				bool operator==(const Iterator & it) {return this->cur == it.cur;};
				bool operator!=(const Iterator & it) {return this->cur != it.cur;};
				T * operator->(void) {T* res = (T*)cur;return res-1;};
				T & operator*(void) {T* res = (T*)cur;return *(res-1);};
			private:
				ListElement * cur;
		};
	
	public:
		DoubleLinkList();//checked
		bool isEmpty(void) const;//checked
		void putFirst(T * value);//checked
		void putLast(T * value);//checked
		static void checkSize(Size size);
		static DoubleLinkList<T> * remove(T * value);
		T * popFirst(void);
		T * popLast(void);
		Iterator begin(void) {Iterator it(root.next); return it;};
		Iterator end(void) {Iterator it(&root); return it;};
	private:
		static DoubleLinkList<T> * remove(ListElement * value);
		DoubleLinkList ( const DoubleLinkList& other );
		DoubleLinkList& operator= ( const DoubleLinkList& other );
	private:
		ListElement root;
};

/********************  HEADERS  *********************/
#include "DoubleLinkList_impl.h"

#endif // DOUBLELINKLIST_H
