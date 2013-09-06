/********************  HEADERS  *********************/
#include "Debug.h"
#include "TraceReader.h"

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
	size_t res = fread(&entry,sizeof(entry),1,fp);

	return (res == 1);
}
