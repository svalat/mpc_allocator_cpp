/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef IDOUBLE_LINK_LIST_ELEMENT_H
#define IDOUBLE_LINK_LIST_ELEMENT_H

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  STRUCT  *********************/
struct ListElement
{
	ListElement() {this->next = this;this->prev = this;};
	ListElement * getListHandler(void) {return this;}
	static ListElement * getFromListHandler(ListElement * list) {return list;}
	ListElement * next;
	ListElement * prev;
};

/*********************  CLASS  **********************/
class IDoubleLinkListeElement
{
	public:
		//virtual ListElement * getListHandler(void) = 0;
		//static MediumChunk::getFromListHandler(ListElement * list);
};

};

#endif //IDOUBLE_LINK_LIST_ELEMENT_H
