#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <stdlib.h>

int main()
{

    glob_t globBuffer;
    int i;

    glob("*.c", GLOB_BRACE, NULL, &globBuffer);

    printf("C and TXT files:\n");
    for(i = 0; i < globBuffer.gl_pathc; i++) {
        printf("%i) %s\n",i+1, globBuffer.gl_pathv[i]);
    }

    globfree(&globBuffer);

    return EXIT_SUCCESS;

}