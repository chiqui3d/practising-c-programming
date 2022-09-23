#include <stdio.h>
#include "macros.h"

int global_variable = 0;

int main(void)
{


    char *s = "HI!";
    printf("%s\n", s);
    printf("%c\n", *s);
    printf("%c\n", *(s + 1));
    printf("%c\n", *(s + 2));

    int *integer = NEW(int,1);
    *integer = 5;

    printf("%i\n", *integer);
    free(integer);

    ++global_variable;
    printf("%i\n", global_variable);

    

    return 0;
}
