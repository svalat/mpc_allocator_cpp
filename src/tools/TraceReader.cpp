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
	this->name = fname;
	
	//open file
	fp = fopen(fname,"r");
	pos = 0;
	
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
void TraceReader::reopen ( void )
{
	//open file
	fp = fopen(name,"r");
	pos = 0;
	
	//check errors
	allocAssumePerror(fp != NULL,"Filed to open file.");
}

/*******************  FUNCTION  *********************/
bool TraceReader::readNext ( TraceEntryComplete& entry )
{
	if (feof(fp))
		return false;
	
	assert(fp != NULL);
	size_t res = fread(&entry,1,sizeof(entry),fp);
	
	if (res >= 0)
		this->pos += sizeof(entry);

	if (res == sizeof(entry.call))
		entry.result = -2;
	
	if (res < sizeof(entry.call) && res > 0)
		allocWarning("Get partial entry");

	return (res == sizeof(entry) || res == sizeof(entry.call));
}

/*******************  FUNCTION  *********************/
bool TraceReader::readPrev ( TraceEntryComplete& entry )
{
	//error
	assert(pos % sizeof(entry) == 0);
	
	//if at begin
	if (pos <= sizeof(entry))
		return false;
	
	//seek
	int res = fseek(fp,pos - 2*sizeof(entry),SEEK_CUR);
	pos -= 2* sizeof(entry);
	
	//cannot seek
	if (res != 0)
		return false;
	else
		return readNext(entry);
}

/*******************  FUNCTION  *********************/
size_t TraceReader::getCurrentId ( void ) const
{
	assert(pos % sizeof(TraceEntryComplete) == 0);
	return pos / sizeof(TraceEntryComplete);
}

/*******************  FUNCTION  *********************/
void TraceReader::setPosition ( size_t id )
{
	pos = id * sizeof(TraceEntryComplete);
	int res = fseek(fp,pos,SEEK_SET);
	allocAssumePerror(res != -1,"Try to move to invalid file position.");
}
