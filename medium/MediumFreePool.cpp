/********************  HEADERS  *********************/
#include "MediumFreePool.h"

/*******************  FUNCTION  *********************/
MediumFreePool::MediumFreePool ( void )
{
	init(cstDefaultFreeSizes,reverseDefaultFreeSizes);
}

/*******************  FUNCTION  *********************/
MediumFreePool::MediumFreePool ( const Size freeSizes[48], ReverseAnalyticFreeSize analyticRevers )
{
	init(freeSizes,analyticRevers);
}

/*******************  FUNCTION  *********************/
void MediumFreePool::init ( const Size freeSizes[48], ReverseAnalyticFreeSize analyticRevers )
{
	assert(freeSizes != NULL);
	assert(this != NULL);
	assert(freeSizes[NB_FREE_LIST-1]==-1);
	
	//setup sizes
	this->sizes = freeSizes;
	this->nbLists = 0;
	this->analyticRevers = analyticRevers;
	
	//count sizes
	for (int i = 0 ; i < NB_FREE_LIST ; i++)
	{
		status[i] = false;
		if (sizes[i] != -1)
			nbLists=i+1;
	}
	
	 //keep the list -1
	if (nbLists < NB_FREE_LIST)
	{
		if (nbLists%2 == 0)
				nbLists+=2;
		else
				nbLists++;
		//TODO
		assert(nbLists < NB_FREE_LIST);
		//assume_m(pool->nb_free_lists < NB_FREE_LIST,"Error while calculating number of free lists.");
	} else {
		//TODO
		assert(false);
		//warning("Caution, the last free list size must be -1, you didn't follow this requirement, this may leed to errors.");
	}
}

/*******************  FUNCTION  *********************/
ChunkFreeList* MediumFreePool::getFreeList ( Size size )
{
	//errors
	assert(this != NULL);
	assert(sizes != NULL);
	assert(size > 0);
	
	if (analyticRevers == NULL)
		return getFreeListByDichotomy(size);
	else
		return getFreeListByAnalytic(size);
}

/*******************  FUNCTION  *********************/
ChunkFreeList* MediumFreePool::getFreeListByDichotomy ( Size size )
{
	//local vars
	Size seg_size = nbLists;
	Size i = seg_size >> 1;
	const Size * ptr = sizes;
	
	//errors
	assert(ptr != NULL);
	assert(this != NULL);
	assert(size > 0);
	assert(4 >> 1 == 2);//required property to quickly divide by 2
	
	if (ptr[0] >= size)
	{
		i = 0;
	} else {
		//use dicotomic search, it's faster as we know the list sizes are sorted.
		while((ptr[i-1] >= size || ptr[i] < size))
		{
			if (ptr[i] < size)
			{
				seg_size -= i;
				ptr += i;
			} else {
				seg_size = i;
			}
			i = seg_size >> 1;//divide by 2
		}
	}
	assert(i >= 0 && ptr+i >= this->sizes);
	
	return lists+(ptr-sizes+i);
}

/*******************  FUNCTION  *********************/
ChunkFreeList* MediumFreePool::getFreeListByAnalytic ( Size size )
{
	//vars
	const Size * sizeList = this->sizes;
	ReverseAnalyticFreeSize analyticRevers = this->analyticRevers;
	int pos;
	
	//errors
	assert(size > 0);
	assert(this != NULL);
	assert(sizeList != NULL);
	assert(analyticRevers != NULL);

	//get position by reverse analytic computation.
	pos = analyticRevers(size, sizeList,nbLists);

	//check size of current cell, if too small, take the next one
	if (sizeList[pos] < size)
		pos++;

	//check
	assert(pos >= 0 && pos <= nbLists);
	assert(lists + pos == getFreeListByDichotomy(size));

	//return position
	return lists + pos;
}

/*******************  FUNCTION  *********************/
Size MediumFreePool::getListClass ( const ChunkFreeList* list ) const
{
	int id = (int)(list - lists);
	#ifndef SCTK_ALLOC_FAST_BUT_LESS_SAFE
	assert(id >= 0 && id < NB_FREE_LIST);
	//assume_m(id >= 0 && id < NB_FREE_LIST,"The given list didn't be a member of the given thread pool.");
	#endif
	return sizes[id];
}

/*******************  FUNCTION  *********************/
void MediumFreePool::insert ( MediumChunk* chunk,ChunkInsertMode mode )
{
	//get size
	chunk->check();
	Size totalSize = chunk->getTotalSize();

	//errors
	assert(this != NULL);
	assert(chunk->getTotalSize() > 0);
	assert(chunk->getStatus() == CHUNK_ALLOCATED);
	
	//get the free list
	ChunkFreeList * flist = getFreeList(totalSize);
	assert(flist != NULL);
	
	Size listClass = getListClass(flist);
	if (flist != lists && listClass != -1 && listClass != totalSize)
		flist--;
	
	//mark free
	chunk->setStatus(CHUNK_FREE);
	
	//insert
	switch(mode)
	{
		case CHUNK_INSERT_FIFO:
			flist->putFirst(chunk);
			break;
		case CHUNK_INSERT_LIFO:
			flist->putLast(chunk);
			break;
		default:
			//TODO
			assert(false);
			//assume_m(false,"Unknown insert mode in free list.");
			break;
	}
	
	//mark non empty
	setEmptyStatus(flist,true);
}

/*******************  FUNCTION  *********************/
void MediumFreePool::insert ( void* ptr, Size size,ChunkInsertMode mode )
{
	MediumChunk * chunk = MediumChunk::setup(ptr,size);
	insert(chunk,mode);
}

/*******************  FUNCTION  *********************/
void MediumFreePool::remove ( MediumChunk* chunk )
{
	//errors
	assert(chunk != NULL);
	assert(this != NULL);
	chunk->check();
	assert(chunk->getStatus() == CHUNK_FREE);
	
	ChunkFreeList * list = ChunkFreeList::remove(chunk);
	if (list != NULL)
		setEmptyStatus(list,false);
	
	chunk->setStatus(CHUNK_ALLOCATED);
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumFreePool::findAdaptedChunk ( ChunkFreeList * list, Size innerSize )
{
	//errors
	assert( innerSize >= 0);
	assert(this != NULL);
	assert( list != NULL);
	assert( list >= lists && list < lists + nbLists);

	//first in the list fo oldest one -> FIFO
	MediumChunk * chunk = list->popFirst();
	ChunkFreeList::Iterator it = list->begin();
	while (it != list->end() && it->getInnerSize() < innerSize );
		++it;
	
	if (it == list->end())
		return NULL;
	else
		return &(*it);
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumFreePool::findChunk ( Size innerSize )
{
	//vars
	MediumChunk * res = NULL;

	//errors
	assert(innerSize > 0);
	assert(this != NULL);
	
	//get the minimum valid size
	ChunkFreeList * list = getFreeList(innerSize);
	ChunkFreeList * startPoint = list;
	
	//if empty list, go to next if available
	//otherwite, take the first of the list (oldest one -> FIFO)
	list = getFirstNextNonEmptyList(list);
	if (list != NULL)
		res = findAdaptedChunk(list,innerSize);
	
	//if not found, try our chance in the previous list (we may find some sufficient bloc, but may
	//require more steps of searching as their may be some smaller blocs in this one on the contrary
	//of our starting point which guaranty to get sufficient size
	if (res == NULL && startPoint != lists)
	{
		list = startPoint - 1;
		res = findAdaptedChunk(list,innerSize);
	}
	
	//if find, remove from list
	if (res != NULL)
		remove(res);
	
	return res;
}

/*******************  FUNCTION  *********************/
ChunkFreeList* MediumFreePool::getFirstNextNonEmptyList ( ChunkFreeList* list )
{
	//errors
	assert(this != NULL);
	assert(list != NULL);
	assert(nbLists <= NB_FREE_LIST);
	assert(list >= lists && list < lists + nbLists);
	
	//get free list id
	Size id = (list - lists);
	assert(id < nbLists);
	
	for (Size i = id ; i < nbLists ; i++)
	{
		if (status[i])
			return lists+i;
	}

	//don't foudna nything non empty
	return NULL;
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumFreePool::merge ( MediumChunk* chunk )
{
	//vars
	MediumChunk * first = chunk;
	MediumChunk * last = chunk;
	MediumChunk * cur;

	//error
	assert(this != NULL);
	assert(chunk != NULL);
	assert(chunk->getStatus() == CHUNK_ALLOCATED);
	//assume_m(chunk->getStatus() == CHUNK_ALLOCATED,"The central chunk must be allocated to be merged.");
	
	//search for the first free chunk before the central one.
	cur = chunk->getPrev();
	while(cur != NULL && cur->getStatus() == CHUNK_FREE)
	{
		first = cur;
		//can remove current one from free list to be merged at the end of the function
		remove(cur);
		//move to next one
		cur = cur->getPrev();
	}

	//search the last mergeable after the central one
	cur = chunk->getNext();
	while(cur != NULL && cur->getStatus() == CHUNK_FREE)
	{
		last = cur;
		//can remove current one from free list to be merged at the end of the function
		remove(cur);
		//move to next one
		last = cur->getNext();
	}
	
	//calc final bloc size
	first->merge(last);
	return first;
}
