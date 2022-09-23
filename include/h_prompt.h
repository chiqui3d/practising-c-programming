#ifndef GET_PROMPT_INCLUDE
#define GET_PROMPT_INCLUDE

#include <stdarg.h>



/**
 * Get the char pointer from stdin
 * 
 * It is necessary to free the memory
 * 
 * @param format 
 * @param ... 
 * 
 * @return char* 
 */
char *getString(char *format, ...);

/**
 * Get the int from stdin
 * 
 * @param format 
 * @param ... 
 * 
 * @return int 
 */
int getInt(char *format, ...);

char *getLine();

#endif