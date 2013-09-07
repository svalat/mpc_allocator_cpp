/********************  HEADERS  *********************/
#include "Debug.h"
#include "TraceReader.h"

/**********************  USING  *********************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
TraceReader::TraceReader ( const char* fname )
{
	//errors
	assert(fname != NULL);
	
	//open file
	fp = fopen(fname,"r");
	
	//check errors
	allocAssumePerror(fp != NULL,"Filed to open file.");
}

/*******************  FUNCTION  *********************/
TraceReader::~TraceReader ( void )
{
	if (fp != NULL)
		fclose(fp);
}

/*******************  FUNCTION  *********************/
bool TraceReader::readNext ( TraceEntryComplete& entry )
{
	if (feof(fp))
		return false;
	
	assert(fp != NULL);
	size_t res = fread(&entry,1,sizeof(entry),fp);
	
	if (res == sizeof(entry.call))
		entry.result = -2;
	
	if (res < sizeof(entry.call) && res > 0)
		allocWarning("Get partial entry");

	return (res == sizeof(entry) || res == sizeof(entry.call));
}
