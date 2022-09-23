#ifndef CHAR_HELPER_H
#define CHAR_HELPER_H

#include <stdbool.h>

#define LOGGER_CURRENT_FILE_DATE_FORMAT "%Y-%m-%d"
#define DEFAULT_LOGGER_FILE_PATH "/var/log/"
#define LOGGER_PATH_MAX 1024
#define LOGGER_FILE_NAME_MAX 256
#define LOGGER_DATE_FORMAT_MAX 11


struct Logger {
    bool active;
    char path[LOGGER_PATH_MAX];
    char testFullPath[LOGGER_PATH_MAX];
    char fileName[LOGGER_FILE_NAME_MAX];
};

extern struct Logger LOGGER;

char *getTestFullPath(char *loggerTestPath);
char *getLoggerFileCurrentDate(char *date);
char *getLoggerFullPath(char *loggerFullPath);
char *getLoggerFileName(char *loggerFileName);



#endif // CHAR_HELPER_H