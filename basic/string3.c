#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void) {
    char mainString[200];
    char string[] = "Hello word";
    char *stringPointer = " Bye word";
    char *stringPointer2 = strdup(stringPointer);

    strcpy(mainString, string);
    strcat(mainString, stringPointer2);
    memset(mainString,'-',5);

    printf("%s\n", mainString);
    free(stringPointer2);

    return 0;
}
