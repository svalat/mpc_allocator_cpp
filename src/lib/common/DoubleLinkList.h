/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef DOUBLELINKLIST_H
#define DOUBLELINKLIST_H

/********************  HEADERS  *********************/
#include "Common.h"
#include "IDoubleLinkListElement.h"
#include <JsonState.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

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
				T * operator->(void) {return T::getFromListHandler(cur);};
				T & operator*(void) {return *T::getFromListHandler(cur);};
			private:
				ListElement * cur;
		};
		
		class ConstIterator
		{
			public:
				ConstIterator(const ListElement * elt) {this->cur = elt;};
				ConstIterator & operator=(const ConstIterator & it) {this->cur = it.cur;};
				ConstIterator & operator++(void) {this->cur = this->cur->next; return *this;};
				ConstIterator & operator--(void) {this->cur = this->cur->prev; return *this;};
				bool operator==(const ConstIterator & it) {return this->cur == it.cur;};
				bool operator!=(const ConstIterator & it) {return this->cur != it.cur;};
				const T * operator->(void) {return T::getFromListHandler((ListElement*)cur);};
				const T & operator*(void) {return *T::getFromListHandler((ListElement*)cur);};
			private:
				const ListElement * cur;
		};
	
	public:
		DoubleLinkList();//checked
		bool isEmpty(void) const;//checked
		void putFirst(T * value);//checked
		void putLast(T * value);//checked
		void hardChecking(void);
		static void checkSize(Size size);
		static DoubleLinkList<T> * remove(T * value);
		T * popFirst(void);
		T * popLast(void);
		T * getFirst(void);
		T * getLast(void);
		Iterator begin(void) {Iterator it(root.next); return it;};
		Iterator end(void) {Iterator it(&root); return it;};
		ConstIterator begin(void) const {ConstIterator it(root.next); return it;};
		ConstIterator end(void) const {ConstIterator it(&root); return it;};
	private:
		static DoubleLinkList<T> * remove(ListElement * value,int /*unused*/);
		DoubleLinkList ( const DoubleLinkList& other );
		DoubleLinkList& operator= ( const DoubleLinkList& other );
	private:
		ListElement root;
	public:
		template <class U> friend void typeToJson(htopml::JsonState& json, std::ostream& stream, const DoubleLinkList<U> & value);
		template <class U> friend void typeToJson(htopml::JsonState& json, std::ostream& stream, const DoubleLinkList<U> & value,const ListElement & elt);
};

};

/********************  HEADERS  *********************/
#include "DoubleLinkList_impl.h"

#endif // DOUBLELINKLIST_H
