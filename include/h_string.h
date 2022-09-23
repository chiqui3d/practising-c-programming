#ifdef _WIN32
#include <string.h>
#define strcasecmp _stricmp
#else // assuming POSIX or BSD compliant system
#include <strings.h>
#endif


#ifndef GET_STRING_INCLUDED 

#define GET_STRING_INCLUDED

#include <stddef.h>

char *str_repeat(const char *str, size_t count);

#endif
