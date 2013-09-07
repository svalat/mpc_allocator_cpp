/********************  HEADERS  *********************/
#include "Debug.h"
#include "CollisionChecker.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CheckerMemorySegment::CheckerMemorySegment ( uint64_t base, uint64_t size )
{
	this->base = base;
	this->size = size;
}

/*******************  FUNCTION  *********************/
CheckerMemorySegment::CheckerMemorySegment ( void * base, size_t size )
{
	this->base = (uint64_t)base;
	this->size = (uint64_t)size;
}

/*******************  FUNCTION  *********************/
CheckerMemorySegment::CheckerMemorySegment ( const CheckerMemorySegment& orig )
{
	this->base = orig.base;
	this->size = orig.size;
}

/*******************  FUNCTION  *********************/
uint64_t CheckerMemorySegment::getBase ( void ) const
{
	return base;
}

/*******************  FUNCTION  *********************/
uint64_t CheckerMemorySegment::getSize ( void ) const
{
	return size;
}

/*******************  FUNCTION  *********************/
bool operator< ( const CheckerMemorySegment& v1, const CheckerMemorySegment& v2 )
{
	return (v1.base + v1.size <= v2.base);
}

/*******************  FUNCTION  *********************/
bool operator>( const CheckerMemorySegment& v1, const CheckerMemorySegment& v2 )
{
	return (v1.base >= v2.base + v2.size);
}

/*******************  FUNCTION  *********************/
bool operator== ( const CheckerMemorySegment& v1, const CheckerMemorySegment& v2 )
{
	return !(v1 > v2 || v1 < v2);
}

/*******************  FUNCTION  *********************/
void CollisionChecker::registerSegment ( void* ptr, size_t size )
{
	if (ptr == NULL)
		return;
	
	CheckerMemorySegment segment(ptr,size);

	std::pair<CheckerMemorySegmentDeque::iterator,bool> res = segments.insert(segment);
	
	if (res.second == false)
		allocFatal("Collision checker detect that the new segment (%p,%lu,%p) conflit with an old active one (%p,%lu,%p)",ptr,size,((char*)ptr+size),res.first->getBase(),res.first->getSize(),res.first->getBase()+res.first->getSize());
}

/*******************  FUNCTION  *********************/
void CollisionChecker::unregisterSegment ( void* ptr, size_t size )
{
	if (ptr == NULL)
		return;

	CheckerMemorySegment segment(ptr,size);
	
	int cnt = segments.erase(segment);
	if (cnt == 0)
		allocFatal("Failed to find segment (%p,%lu) to remove it from collision checker.",ptr,size);
}
