

#include <ctype.h>  // for tolower()
#include <limits.h> // for CHAR_MAX
#include <stdio.h>  // for fprintf()
#include <stdlib.h> // for exit()
#include <string.h> // for strlen()

struct char_struct {
    const char *string;
};


int main() {

    const char *fullPath = "/var/log/filename.log";

    printf("Full path: %s\n\n", fullPath);

    char fileName[CHAR_MAX];

    strcpy(fileName, strrchr(fullPath, '/'));

    struct char_struct charStruct = {0};

    charStruct.string = fileName;

    printf("Filename: %s \n\n", charStruct.string);
    // version 1

    charStruct.string = "filename2.log";

    printf("Filename2: %s \n\n", charStruct.string);

    return EXIT_SUCCESS;
}