/********************  HEADERS  *********************/
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "Debug.h"
#include "OS.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CONSTS  *********************/
static const char * cstMessageLevelStr[] = {"Debug message" , "Info"            , "Trace"                    ,
                                            "Warning"       , "Error"           , "Error"                    , 
                                            "Fatal error"   , "Fatal error"     , "Assert failed"            };
static const int    cstMessageLevelFD [] = {OS_STDERR_FILENO, OS_STDERR_FILENO, OS_STDERR_FILENO, OS_STDERR_FILENO, 
                                            OS_STDERR_FILENO, OS_STDERR_FILENO, OS_STDERR_FILENO, OS_STDERR_FILENO,
                                            OS_STDERR_FILENO};
static const char * cstMessageColor   [] = {COLOR_CYAN      , COLOR_BLUE      , COLOR_BLUE      , COLOR_YELLOW    , 
                                            COLOR_RED       , COLOR_RED       , COLOR_RED       , COLOR_RED       , 
                                            COLOR_MAGENTA   };

/********************  MACRO  ***********************/
#define BUFFER_SIZE (4*4096)

/*******************  FUNCTION  *********************/
DebugMessage::DebugMessage ( MessageLevel level,const char * file, const char * func,int line )
{
	this->level = level;
	this->file = file;
	this->func = func;
	this->line = line;
	
	assert(level <= sizeof(cstMessageLevelFD) / sizeof(cstMessageLevelFD[0]));
}

/*******************  FUNCTION  *********************/
void DebugMessage::print ( const char* format, ... )
{
	//prepare buffers
	va_list param;
	char loc [BUFFER_SIZE];
	char glob[BUFFER_SIZE];
	
	//prepare the location line
	size_t size;
	if (level == MESSAGE_TRACE)
		size = sprintf(loc,"%s%s : %s" DEFAULT_COLOR "\n",cstMessageColor[level],cstMessageLevelStr[level],format);
	else
		size = sprintf(loc,"%s%s at %s:%d (%s)\n%s" DEFAULT_COLOR "\n",cstMessageColor[level],cstMessageLevelStr[level],file,line,func,format);
	assert(size <= sizeof(loc));
	
	//final line
	va_start (param, format);
	size = vsprintf(glob,loc,param);
	assert(size <= sizeof(glob));
	va_end (param);
	
	//print mode
	if (level == MESSAGE_PERROR || level == MESSAGE_FATAL_PERROR)
		perror(glob);
	else
		OS::safeWrite(cstMessageLevelFD[level],glob,size);
	
	//special
	if (level >= MESSAGE_FATAL && level != MESSAGE_ASSERT)
		abort();
}

};
