// C implementation to see how errno value is
// set in the case of any error in C
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// #include <setjmp.h>

#include "cexception/lib/CException.c"

CEXCEPTION_T NO_SUCH_FILE_DIRECTORY = 1;

void f()
{
    Throw(NO_SUCH_FILE_DIRECTORY);
}

int main(void)
{
    // If a file is opened which does not exist,
    // then it will be an error and corresponding
    // errno value will be set
    FILE *fp;

    // opening a file which does not exist.
    fp = fopen("GeeksForGeeks.txt", "r");

    printf("errno code: %d\n ", errno);
    perror("Error printed by perror");
    fprintf(stderr, "meaningful message here\n");

    Try {
     f();
   }
   Catch(NO_SUCH_FILE_DIRECTORY) {
    printf("Error printed with try catch, Error code: %i, Error message: %s\n",NO_SUCH_FILE_DIRECTORY, strerror(errno));
   }

   /* float fx;
    if (x==0)
    {
        printf("Division by Zero is not allowed");
        fprintf(stderr, "Division by zero! Exiting...\n");
        exit(EXIT_FAILURE);
    }*/

    exit(EXIT_FAILURE);
}
