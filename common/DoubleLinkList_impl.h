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

/*******************  FUNCTION  *********************/
template <class T>
void typeToJson(htopml::JsonState& json, std::ostream& stream, const DoubleLinkList<T> & value)
{
// 	json.openStruct();
// 	json.openField("root");
	typeToJson(json,stream,value,value.root);
// 	json.closeField("root");
// 	json.closeStruct();
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
template <class T>
void typeToJson(htopml::JsonState& json, std::ostream& stream, const DoubleLinkList<T> & value,const ListElement & elt)
{
	json.openStruct();
	typeToJsonInner(json,stream,value,elt);
	
	///////////////////////////////
	json.openField("__mem_objects__");
	json.openArray();
	const ListElement * cur = elt.next;
	bool first = true;
	while (cur != &elt)
	{
		if (!first)
			stream << ",";
		first = false;
		json.openStruct();
		typeToJsonInner(json,stream,value,*cur);
		json.closeStruct();
		cur = cur->next;
	}
	json.closeArray();
	json.closeField("__mem_objects__");
	///////////////////////////////
	
	///////////////////////////////
	json.openField("__mem_links__");
	json.openArray();
	cur = &elt;
	first = true;
	while (cur != &elt || first == true)
	{
		if (!first)
			stream << ",";
		first = false;
		json.openStruct();
		json.printField("from",(void*)cur);
		json.printField("to",(void*)cur->next);
		json.closeStruct();
		if (cur->next != cur)
		{
			stream << ",";
			json.openStruct();
			json.printField("from",(void*)cur);
			json.printField("to",(void*)cur->prev);
			json.closeStruct();
			cur = cur->next;
		}
		if (cur != &elt)
		{
			T * tmp = ((T*)cur)-1;
			stream << ",";
			json.openStruct();
			json.printField("from",(void*)cur);
			json.printField("to",(void*)tmp);
			json.closeStruct();
		}
	}
	json.closeArray();
	json.closeField("__mem_links__");
	///////////////////////////////
	
	json.closeStruct();
}

#endif //DOUBLELINKLIST_IMPL_H
