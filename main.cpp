#include <iostream>
#include <cassert>
#include <iostream>
#include "MediumChunk.h"
#include <MediumFreePool.h>
#include "StdPool.h"

int main(int argc, char **argv) {
	//check sizes
	assert(sizeof(Size) == sizeof(size_t));
	assert(sizeof(Addr) == sizeof(void*));
	assert(sizeof(MediumChunk) == 3 * 8);
	
	char buffer[4096];
	
	MediumChunk * c0 = MediumChunk::setup(buffer,4096);
	MediumChunk * c1 = c0->split(16);
	MediumChunk * c2 = c1->split(16);
	MediumChunk * c3 = c2->split(16);
	c3 = c3->split(16);
	c3 = c3->split(16);
	c3 = c3->split(16);
	
	MediumFreePool pool;
	pool.insert(c0,CHUNK_INSERT_LIFO);
	pool.insert(c1,CHUNK_INSERT_LIFO);
	pool.insert(c2,CHUNK_INSERT_LIFO);
	
	htopml::JsonState state(&std::cout);
	state.openStruct();
	state.printField("c0",*c0);
	state.printField("pool",pool);
	state.closeStruct();
}
