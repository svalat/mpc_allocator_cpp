#ifndef OS_H
#define OS_H

/********************  HEADERS  *********************/
#include <cstdlib>

/********************  MACRO  ***********************/
#define OS_STDERR_FILENO 2
#ifdef HAVE_COLOR
	#define DEFAULT_COLOR    "\033[0m"
	#define COLOR_RED        "\033[31m"
	#define COLOR_GREEN      "\033[32m"
	#define COLOR_YELLOW     "\033[33m"
	#define COLOR_BLUE       "\033[34m"
	#define COLOR_MAGENTA    "\033[35m"
	#define COLOR_CYAN       "\033[36m"
	#define COLOR_WHITE      "\033[37m"
#else
	#define DEFAULT_COLOR    ""
	#define COLOR_RED        ""
	#define COLOR_GREEN      ""
	#define COLOR_YELLOW     ""
	#define COLOR_BLUE       ""
	#define COLOR_MAGENTA    ""
	#define COLOR_CYAN       ""
	#define COLOR_WHITE      ""
#endif

/*********************  CLASS  **********************/
class OSUnix
{
	public:
		static void * mmap( void* addr, size_t size );
		static int munmap( void* addr, size_t size );
		static size_t safeWrite(int fd,const char * value,size_t size);
};

/*********************  CLASS  **********************/
class OSLinux : public OSUnix
{
	public:
		static void * mremap( void * addr, size_t old_size, size_t new_size,void * dest_addr = NULL);
};

/*********************  TYPES  **********************/
typedef OSLinux OS;

#endif //OS_H
