#include <stdlib.h>

#ifdef __cplusplus
# define NEW(type, count) ((type *)calloc(count, sizeof(type)))
#else
# define NEW(type, count) (calloc(count, sizeof(type)))
#endif

extern int global_variable;

