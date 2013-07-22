#ifndef COMMON_H
#define COMMON_H

/********************  HEADERS  *********************/
#include <cstdlib>
#include <cassert>
#include <stdint.h>

/*********************  TYPES  **********************/
typedef uint64_t Addr;
typedef uint64_t Size;

/********************  MACRO  ***********************/
#define BASIC_ALIGN sizeof(Addr)
#define MAGICK_VALUE 42
#define NB_FREE_LIST 50
#define UNSUPPORTED -1
#define ALLOC_MIN_SIZE BASIC_ALIGN
//TODO setup value
#define REALLOC_THREASHOLD 64
#define SCTK_MACRO_BLOC_SIZE (2*1024*1024)

/*******************  FUNCTION  *********************/
static inline Size ceilToPowOf2(Size size,Size align)
{
	size &= ~(align-1);
	return size;
}

/*******************  FUNCTION  *********************/
static inline Size upToPowOf2(Size size,Size align)
{
	if (size & (align-1))
		size = (size & ~(align-1))+align;
	else
		size &= ~(align-1);
	return size;
}

/*******************  FUNCTION  *********************/
template <class T>
T * addrOffset(T * value,Size offset)
{
	assert(value != NULL);
	return (T*)((Addr)value + offset);
}

/*******************  FUNCTION  *********************/
template <class T>
T max(T a,T b)
{
	if (a>b)
		return a;
	else
		return b;
}

#endif //COMMON_H
