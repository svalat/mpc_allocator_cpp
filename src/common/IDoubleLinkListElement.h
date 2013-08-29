#ifndef IDOUBLE_LINK_LIST_ELEMENT_H
#define IDOUBLE_LINK_LIST_ELEMENT_H

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

#endif //IDOUBLE_LINK_LIST_ELEMENT_H
