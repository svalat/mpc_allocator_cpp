#ifndef DOUBLELINKLIST_IMPL_H
#define DOUBLELINKLIST_IMPL_H

/********************  HEADERS  *********************/
#include "DoubleLinkList.h"
#include <cassert>
#include <TypeToJson.h>

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
	ListElement * elt = value->getListHandler();
	elt->next = root.next;
	elt->prev = &root;
	
	root.next->prev = elt;
	root.next = elt;
}

/*******************  FUNCTION  *********************/
template <class T>
void DoubleLinkList<T>::putLast ( T* value)
{
	ListElement * elt = value->getListHandler();
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
	
	return T::getFromListHandler(elt);
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
	
	return T::getFromListHandler(elt);
}

/*******************  FUNCTION  *********************/
template <class T>
DoubleLinkList<T> * DoubleLinkList<T>::remove(T * value)
{
	assert(value != NULL);
	ListElement * elt = value->getListHandler();
	
	return remove(elt,0);
}


/*******************  FUNCTION  *********************/
template <class T>
DoubleLinkList<T> * DoubleLinkList<T>::remove(ListElement * value,int /*unused*/)
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

/*******************  FUNCTION  *********************/
template <class T>
void typeToJsonInner(htopml::JsonState& json, std::ostream& stream, const DoubleLinkList<T> & value,const ListElement & elt)
{
	json.printField("__mem_address__",(void*)&elt);
	json.printField("__class_name__","Element");
	json.printField("next",(void*)elt.next);
	json.printField("prev",(void*)elt.prev);
}

/*******************  FUNCTION  *********************/
template <class T> void typeToJson(htopml::JsonState & json,std::ostream& stream, const DoubleLinkList<T> & iterable)
{
	json.openArray();

	for (typename DoubleLinkList<T>::ConstIterator it = iterable.begin() ; it != iterable.end() ; ++it)
		json.printValue(*it);

	json.closeArray();
}

#endif //DOUBLELINKLIST_IMPL_H
