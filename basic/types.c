#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/ctypes.h"


int main(void)
{
    
   char hello = 'H';

    if (typecheck(int, hello)){
        printf("%s\n", "hello is an int");
    } else {
        printf("%s\n", "hello is not an int");
    }

    if (typecheck(char, hello)){
        printf("%s\n", "hello is a char");
    } else {    
        printf("%s\n", "hello is not a char");
    }

     printf("size_t is '%s'\n", typename(hello));

    return EXIT_SUCCESS;

}
