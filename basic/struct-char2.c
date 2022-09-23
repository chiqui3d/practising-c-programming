

#include <ctype.h>  // for tolower()
#include <limits.h> // for CHAR_MAX
#include <stdio.h>  // for fprintf()
#include <stdlib.h> // for exit()
#include <string.h> // for strlen()

struct CONFIG {
    const char *fileName;
};

struct CONFIG CONFIG_GLOBAL = {0};


char *getFileName(char *fileName){

    strcpy(fileName, "2022-01-01");
    strcat(fileName, ".log");

    return fileName;
}

char *getFileNameFromConfigGlobal(char *fileName){

    strcpy(fileName, CONFIG_GLOBAL.fileName);

    return fileName;
}

int main() {


    struct CONFIG config = {0};

    char fileName[CHAR_MAX];
    getFileName(fileName);
    printf("Filename: %s \n\n", fileName);

    config.fileName = fileName;

    CONFIG_GLOBAL = config;

    printf("Filename: %s \n\n", CONFIG_GLOBAL.fileName);

    char fileNameFromGlobal[CHAR_MAX];

    getFileNameFromConfigGlobal(fileNameFromGlobal);

    printf("Filename from global: %s \n\n", fileNameFromGlobal);

    return EXIT_SUCCESS;
}