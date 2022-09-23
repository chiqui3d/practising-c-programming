#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char-helper.h"

struct Logger LOGGER;

int main(void) {
    struct Logger logger;
    memset(&logger, 0, sizeof(struct Logger));
    logger.active = false;

    strcpy(logger.path, DEFAULT_LOGGER_FILE_PATH);
    printf("Path: %s\n", logger.path); // worked

    char loggerTestFullPath[LOGGER_PATH_MAX];
    getTestFullPath(loggerTestFullPath);                 // worked
    strcpy(logger.testFullPath, loggerTestFullPath);     // worked
    printf("Test Full Path: %s\n", logger.testFullPath); // worked

     
    /* -------------------------------------------------------------- */
    // same that above but not worked 
    char loggerFileName1[LOGGER_FILE_NAME_MAX];
    getLoggerFileName(loggerFileName1);        // not worked
    strcpy(logger.fileName, loggerFileName1);  // not worked
    printf("FileName: %s\n", logger.fileName); // not worked


  /*   char loggerFileName2[LOGGER_FILE_NAME_MAX];
    strcpy(logger.fileName, getLoggerFileName(loggerFileName2)); // worked
    printf("FileName: %s\n", logger.fileName);                   // worked */

    return EXIT_SUCCESS;
}