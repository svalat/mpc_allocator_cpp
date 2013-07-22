#include <iostream>
#include <cassert>
#include "MediumChunk.h"
#include "StdPool.h"

int main(int argc, char **argv) {
	//check sizes
	assert(sizeof(Size) == sizeof(size_t));
	assert(sizeof(Addr) == sizeof(void*));
	assert(sizeof(MediumChunk) == 3 * 8);
}