#ifndef DOUBLELINKLIST_IMPL_H
#define DOUBLELINKLIST_IMPL_H

/********************  HEADERS  *********************/
#include "DoubleLinkList.h"
#include <cassert>

/*******************  FUNCTION  *********************/
inline ListElement::ListElement()
{
	this->next = this;
	this->prev = this;
}

/*******************  FUNCTION  *********************/
template <class T>
void DoubleLinkList<T>::checkSize ( Size size )
{
	assert(size >= sizeof(ListElement));
}

/*******************  FUNCTION  *********************/
template <class T>
DoubleLinkList<T>::DoubleLinkList()
{
}

/*******************  FUNCTION  *********************/
template <class T>
bool DoubleLinkList<T>::isEmpty ( void ) const
{
	if (root.next == &root)
		assert(root.prev == root.next);
	return (root.next == &root);
}

/*******************  FUNCTION  *********************/
template <class T>
void DoubleLinkList<T>::putFirst ( T* value)
{
	ListElement * elt = (ListElement*)(value+1);
	elt->next = root.next;
	elt->prev = &root;
	
	root.next->prev = elt;
	root.next = elt;
}

/*******************  FUNCTION  *********************/
template <class T>
void DoubleLinkList<T>::putLast ( T* value)
{
	ListElement * elt = (ListElement*)(value+1);
	elt->next = &root;
	elt->prev = root.prev;
	
	root.prev->next = elt;
	root.prev = elt;
}

/*******************  FUNCTION  *********************/
template <class T>
T* DoubleLinkList<T>::popFirst ( void )
{
	//TODO optimize
	if (isEmpty())
		return NULL;
	
	ListElement * elt = root.next;
	assert(elt != &root);
	
	root.next = elt->next;
	elt->next->prev = &root;
	
	T * res = (T*)elt;
	return res-1;
}

/*******************  FUNCTION  *********************/
template <class T>
T* DoubleLinkList<T>::popLast ( void )
{
	//TODO optimize
	if (isEmpty())
		return NULL;
	
	ListElement * elt = root.prev;
	assert(elt != &root);
	
	root.prev = elt->prev;
	elt->prev->next = &root;
	
	T * res = (T*)elt;
	return res-1;
}

/*******************  FUNCTION  *********************/
template <class T>
DoubleLinkList<T> * DoubleLinkList<T>::remove(T * value)
{
	assert(value != NULL);
	ListElement * elt = (ListElement*)(value+1);
	
	return remove(elt);
}


/*******************  FUNCTION  *********************/
template <class T>
DoubleLinkList<T> * DoubleLinkList<T>::remove(ListElement * value)
{
	DoubleLinkList<T> * res = NULL;

	assert(value->next->prev == value);
	assert(value->prev->next == value);
	
	value->next->prev = value->prev;
	value->prev->next = value->next;
	
	if (value->prev == value->next)
		res = (DoubleLinkList<T> *)value->next;
	
	#ifndef OPTIMIZED
	value->next = NULL;
	value->prev = NULL;
	#endif
	
	return res;
}
#endif //DOUBLELINKLIST_IMPL_H
