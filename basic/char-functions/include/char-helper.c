
#include <stdio.h>
#include <stdlib.h> // for exit()
#include <string.h> // for strlen()
#include <time.h> // for time()

#include "char-helper.h"



char *getTestFullPath(char *loggerTestFullPath) {

    strcpy(loggerTestFullPath, "/var/log/test/");
    strcat(loggerTestFullPath, "test.log");

    return loggerTestFullPath;
}

char *getLoggerFileCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);


    if (strftime(date, LOGGER_DATE_FORMAT_MAX, LOGGER_CURRENT_FILE_DATE_FORMAT, tm) == 0) {
        fprintf(stderr, "strftime returned 0");
        exit(EXIT_FAILURE);
    }


    return date;
}

char *getLoggerFileName(char *loggerFileName) {
    char loggerDateName[LOGGER_DATE_FORMAT_MAX ];

    // sprintf(loggerFileName, "%s.log", date);
    
    getLoggerFileCurrentDate(loggerDateName);
    strcpy(loggerFileName, loggerDateName);
    strcat(loggerFileName, ".log");


    return loggerFileName;
}


char *getLoggerFullPath(char *loggerFullPath) {

    strcpy(loggerFullPath, LOGGER.path);
    strcat(loggerFullPath, LOGGER.fileName);

    return loggerFullPath;
}