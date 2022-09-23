#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/h_error.h"

#define CSV_LINE_MAX 1024

int getTokens(char *str, char *delimiter, char ***tokens) {

    int num = 0;

    char *token = strtok(str, delimiter);
    while (token != NULL) {
        char **tmpToken = realloc(*tokens, sizeof(char *) * (num + 1));
        if (!tmpToken) {
            free(tmpToken);
            die("Memory allocation failed");
        }
        *tokens = tmpToken;
        (*tokens)[num] = token;
        num++;
        token = strtok(NULL, delimiter);
    }
    return num;
}

int main(void) {
    // Open CSV file
    FILE *file = fopen("files/csv/test.csv", "a+");
    if (!file) {
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_SET);

    char line[CSV_LINE_MAX];
    size_t count = 0;
    char title[CSV_LINE_MAX];
    char description[CSV_LINE_MAX];

    while (fgets(line, CSV_LINE_MAX, file)) {

        char *str = strdup(line);
        char **tokens = NULL;
        const size_t numTokens = getTokens(str, ";", &tokens);
        if (numTokens == 0) {
            if (tokens) {
                free(tokens);
            }
            free(str);
            continue;
        }
        char tmpToken[CSV_LINE_MAX] = {0};
        size_t nFields = 0, i;
        for (i = 0; i < numTokens; i++) {
            const size_t len = strlen(tokens[i]);
            char field[len + 1];
            strncpy(field, tokens[i], len + 1);
            const size_t lenTmp = strlen(tmpToken);
            if (lenTmp == 0 && tokens[i][0] == '"' && tokens[i][len - 1] != '"') {
                strncpy(tmpToken, field, len);
                continue;
            }
            if (lenTmp > 0) {
                strcat(tmpToken, field);
                if (tokens[i][len - 1] == '"') {
                    strcpy(field, tmpToken);
                    tmpToken[0] = '\0';
                } else {
                    continue;
                }
            }
            printf("%s\n", field);
            nFields++;
        }
        free(tokens);
        free(str);
        count++;
    }

    printf("\nCSV Lines: %zi\n", count);

    // Close file
    fclose(file);
}
