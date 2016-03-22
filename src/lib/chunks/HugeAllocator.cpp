/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include "Debug.h"
#include "HugeAllocator.h"
#include "PaddedChunk.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
HugeAllocator::HugeAllocator ( IMMSource* memorySource )
{
	allocAssert(memorySource != NULL);
	this->memorySource = memorySource;
}

/*******************  FUNCTION  *********************/
void HugeAllocator::rebindMMSource(IMMSource* mmSource)
{
	this->memorySource = mmSource;
}

/*******************  FUNCTION  *********************/
void HugeAllocator::free ( void* ptr )
{
	//trivial
	if (ptr == NULL)
		return;
	
	//remove padding
	ptr = PaddedChunk::unpad(ptr);

	//TODO make a safe version of this function with checking (if possible)
	RegionSegmentHeader * segment = RegionSegmentHeader::getSegment(ptr);

	//return it to mm source
	memorySource->unmap(segment);
}

/*******************  FUNCTION  *********************/
void* HugeAllocator::malloc ( size_t size, size_t align, bool* zeroFilled )
{
	bool zero = false;
	size_t checkedSize = size;

	//errors
	allocAssert(this != NULL);
	allocAssert(align >= BASIC_ALIGN);
	
	//trivial
	if ( checkedSize == 0)
		return NULL;
	else if (checkedSize < MEDIUM_MIN_INNER_SIZE)
		checkedSize = MEDIUM_MIN_INNER_SIZE;
	
	//zero
	if (zeroFilled != NULL)
		zero = *zeroFilled;
	
	//add place for padding
	if (align > BASIC_ALIGN)
		checkedSize += align;
	
	//request memory to mm source
	RegionSegmentHeader * segment = memorySource->map(checkedSize,&zero,this);
	allocCondWarning(segment != NULL,"Caution, get OOM in huge allocation method.");
	
	//setup zero
	if (zeroFilled != NULL)
		*zeroFilled = zero;

	//ok this is good get ptr
	void * res = segment->getPtr();
	if (res == NULL)
		return NULL;
	
	//check for padding
	if ((Addr)res % align != 0)
		res = PaddedChunk::setup(segment,PaddedChunk::calcPadding(segment,align,size))->getPtr();
	
	//final check
	allocAssert((Addr)res % align == 0);
	allocAssert(res != NULL && segment->contain(res) && segment->contain(addrOffset(res,size-1)));
	
	return res;
}

/*******************  FUNCTION  *********************/
size_t HugeAllocator::getInnerSize ( void* ptr )
{
	//trivial
	if (ptr == NULL)
		return 0;
	
	//unpadd
	void * realPtr = PaddedChunk::unpad(ptr);
	allocAssert(realPtr <= ptr);
	int delta = addrDelta(ptr,realPtr);
	allocAssert(delta >= 0);
	
	RegionSegmentHeader * segment = RegionSegmentHeader::getSegment(ptr);
	if (segment == NULL)	
		return 0;
	else
		return segment->getInnerSize() - delta;
}

/*******************  FUNCTION  *********************/
size_t HugeAllocator::getRequestedSize ( void* ptr )
{
	return UNSUPPORTED;
}

/*******************  FUNCTION  *********************/
size_t HugeAllocator::getTotalSize ( void* ptr )
{
	//trivial
	if (ptr == NULL)
		return 0;
	
	//unpadd
	ptr = PaddedChunk::unpad(ptr);
	
	RegionSegmentHeader * segment = RegionSegmentHeader::getSegment(ptr);
	if (segment == NULL)	
		return 0;
	else
		return segment->getTotalSize();
}

/*******************  FUNCTION  *********************/
void HugeAllocator::hardChecking ( void )
{
	//TODO
}

/*******************  FUNCTION  *********************/
bool HugeAllocator::isThreadSafe ( void ) const
{
	return true;
}

/*******************  FUNCTION  *********************/
void HugeAllocator::remoteFree ( void* ptr )
{
	allocFatal("May not be used as HugeAllocator is thread sage.");
	return this->free(ptr);
}

/*******************  FUNCTION  *********************/
void* HugeAllocator::realloc ( void* ptr, size_t size )
{
	void * oldPtr = ptr;

	//trivial
	if (ptr == NULL && size == 0)
	{
		return NULL;
	} else if (ptr == NULL) {
		return this->malloc(size);
	} else if (size == 0) {
		this->free(ptr);
		return NULL;
	}
	
	//check if padded
	ptr = PaddedChunk::unpad(ptr);
	
	//get old size
	RegionSegmentHeader * segment = RegionSegmentHeader::getSegment(ptr);
	allocAssert(segment != NULL);
	//TODO assume
	Size oldSize = segment->getInnerSize();
	Size delta = oldSize - size;
	
	//if can resuse old one without resize
	if (oldSize >= size && delta <= REALLOC_THREASHOLD)
		return oldPtr;
	
	//remap
	RegionSegmentHeader * newSegment = memorySource->remap(segment,size,this);
	allocCondWarning(newSegment != NULL,"Get OOM in realloc of huge segment.");
	allocAssert(newSegment->getInnerSize() >= size);
	
	return newSegment->getPtr();
}

};
