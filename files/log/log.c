#include <errno.h>
#include <glob.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "./log.h"
#include "../../include/h_error.h"
#include "../../include/h_prompt.h"


char *actionName(enum LogActions action) {
    char *strings[] = {"List", "Search", "Write"};

    return strings[action];
}

int main(void) {

    printf("Logs actions:\n");
    printf("1) List\n");
    printf("2) Seach\n");
    printf("3) Write\n");

    int action = getInt("Enter the action number:");
    enum LogActions logAction = action - 1;

    FILE *logFile = openFile("./_data/log.log");

    switch (logAction) {
        case LIST:
            listLog(logFile);
            break;
        case SEARCH: {
            char *string = getString("Enter the string to search for:");
            searchLog(logFile, string);
            break;
        }
        case WRITE: {
            char *line = getString("Enter the log line:");
            writeLog(logFile, line);
            break;
        }
    }

    // writeLog(logFile, "Los logs revelan donde podría estar el posible
    // fallo");

    return EXIT_SUCCESS;
}

void searchLog(FILE *file, char *string) {

    fseek(file, 0, SEEK_SET);

    int count = 0;
    char buffer[CHAR_MAX];

    while (fgets(buffer, CHAR_MAX, file)) {
        char *found = strcasestr(buffer, string);

        if (found != NULL) {
            printf("%s\n", buffer);
            count++;
        }
    }

    printf("Results found for \"%s\": %i\n", string, count);

    fclose(file);
}

void listLog(FILE *file) {

    fseek(file, 0, SEEK_SET);

    char line[CHAR_MAX];

    while (fgets(line, CHAR_MAX, file)) {
        puts(line);
    }

    fclose(file);
}

FILE *openFile(char *path) {

    char *dirCopy, *baseCopy, *directory, *fileName;

    dirCopy = strdup(path);
    baseCopy = strdup(path);
    directory = dirname(dirCopy);
    fileName = basename(baseCopy);

    if (mkdir(directory, 755) == -1 && errno != EEXIST) {
        die("mkdir");
    }

    free(dirCopy);
    free(baseCopy);

    FILE *pr_file = fopen(path, "a+");
    if (NULL == pr_file) {
        die("fopen");
    }

    return pr_file;
}

void writeLog(FILE *file, char *line) {

    fseek(file, 0, SEEK_END);

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char datetime[100];

    strftime(datetime, sizeof(datetime) + 1, "%Y-%m-%d %H:%M:%S %z", tm);

    char message[CHAR_MAX];
    sprintf(message, "%s - INFO - %s\n", datetime, line);

    printf("%s", message);
    fprintf(file, "%s", message);
    fclose(file);
}