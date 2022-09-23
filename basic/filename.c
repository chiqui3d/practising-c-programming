

#include <stdio.h>  // for fprintf()
#include <stdlib.h> // for exit()
#include <string.h> // for strlen()
#include <ctype.h> // for tolower()
#include <limits.h> // for CHAR_MAX

int main() {


    const char *fullPath = "/var/log/filename.log";

    printf("Full path: %s\n\n", fullPath);

    const char *fileName = strrchr(fullPath, '/');

    printf("File Name: %s\n\n", fileName);

    size_t lenPath = fileName - fullPath;

    printf("Length path: %lu \n\n", lenPath);

    // version 1
    char *path = malloc(lenPath + 1);
    if (path == NULL)
    {
        return 1;
    }
    memcpy(path, fullPath, lenPath);
    path[lenPath] = '\0';

    printf("Path: %s\n\n\n", path);

    free(path);

    // version 2
    char path2[lenPath+1];
    strncpy(path2, fullPath, lenPath);
    path2[lenPath] = '\0';
    printf("Path: %s\n", path2);


    return EXIT_SUCCESS;
}