#ifndef GET_LINE_INCLUDED /* GET_LINE_INCLUDED */

#define GET_LINE_INCLUDED
#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

#include <stddef.h>

int get_line(char *prompt_message, char *buff, size_t sz);


#endif /* GET_LINE_INCLUDED */
