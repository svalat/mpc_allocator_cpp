#ifndef DOUBLELINKLIST_IMPL_H
#define DOUBLELINKLIST_IMPL_H

/********************  HEADERS  *********************/
#include "DoubleLinkList.h"
#include <Debug.h>
#include <TypeToJson.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
template <class T>
void DoubleLinkList<T>::checkSize ( Size size )
{
	allocAssert(size >= sizeof(ListElement));
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
		allocAssert(root.prev == root.next);
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
T * DoubleLinkList<T>::getFirst( void )
{
	if (isEmpty())
		return NULL;
	
	return T::getFromListHandler(root.next);
}

/*******************  FUNCTION  *********************/
template <class T>
T * DoubleLinkList<T>::getLast( void )
{
	if (isEmpty())
		return NULL;
	
	return T::getFromListHandler(root.prev);
}

/*******************  FUNCTION  *********************/
template <class T>
T* DoubleLinkList<T>::popFirst ( void )
{
	//TODO optimize
	if (isEmpty())
		return NULL;
	
	ListElement * elt = root.next;
	allocAssert(elt != &root);
	
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
	allocAssert(elt != &root);
	
	root.prev = elt->prev;
	elt->prev->next = &root;
	
	return T::getFromListHandler(elt);
}

/*******************  FUNCTION  *********************/
template <class T>
DoubleLinkList<T> * DoubleLinkList<T>::remove(T * value)
{
	allocAssert(value != NULL);
	ListElement * elt = value->getListHandler();
	
	return remove(elt,0);
}


/*******************  FUNCTION  *********************/
template <class T>
DoubleLinkList<T> * DoubleLinkList<T>::remove(ListElement * value,int /*unused*/)
{
	DoubleLinkList<T> * res = NULL;

	allocAssert(value->next->prev == value);
	allocAssert(value->prev->next == value);
	
	value->next->prev = value->prev;
	value->prev->next = value->next;
	
	if (value->prev == value->next)
		res = (DoubleLinkList<T> *)value->next;
	
	#ifndef OPTIMIZED
	value->next = value;
	value->prev = value;
	#endif
	
	return res;
}

/*******************  FUNCTION  *********************/
template <class T>
void DoubleLinkList<T>::hardChecking(void)
{
	ListElement * cur = &this->root;
	
	do {
		allocAssert(cur->next->prev == cur);
		allocAssert(cur->prev->next == cur);
		cur = cur->next;
	} while (cur != &this->root);
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

};

#endif //DOUBLELINKLIST_IMPL_H
