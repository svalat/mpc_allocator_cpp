#ifndef PORTABILITY_OS_H
#define PORTABILITY_OS_H

/********************  HEADERS  *********************/
#include <cstdlib>

/********************  MACRO  ***********************/
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

/*********************  TYPES  **********************/
#ifdef OS_Linux
	#include "OSLinux.h"
	typedef OSLinux OS;
#else
	#error "Unsupported OS portability mode"
#endif

#endif //PORTABILITY_OS_H
