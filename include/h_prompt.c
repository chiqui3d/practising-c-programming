#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "h_prompt.h"


char *getString(char *format, ...) {

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  printf("\n");

  char input[CHAR_MAX];
  char stdinTmp[CHAR_MAX];
  // Why malloc?
  // array will be lost by the end of the function, So we created a char *
  // This is allocated on the stack of this function and is no
  // longer valid after this function returns
  // Other option: With a static variable or const char *, there would be no need to allocate dynamic memory. 
  char *stdinString = malloc(CHAR_MAX);
  if (NULL == stdinString) {
    perror("stdinString malloc() failed");
    free(stdinString);
    exit(EXIT_FAILURE);
  }

  while (true) {
    // VS https://stackoverflow.com/questions/22330969/using-fscanf-vs-fgets-and-sscanf
    fgets(input, CHAR_MAX, stdin);
    // until '\n' newline is encountered
    if (sscanf(input, "%[^\n]", stdinTmp)) {
      stdinString = (char *)realloc(stdinString, strlen(stdinTmp) + 1);
      strcpy(stdinString, stdinTmp);
      break;
    }
  }

  return stdinString;
}

int getInt(char *format, ...) {

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  printf("\n");

  char input[CHAR_MAX];
  int stdinInt;
  short int ss;

  while (true) {
    fgets(input, CHAR_MAX, stdin);
    ss = sscanf(input, "%i", &stdinInt);
    if (ss == 1 && stdinInt > 0) {
      break;
    }
  }

  return stdinInt;
}

char *getLine() {

    int c;
    /* what is the buffer current size? */
    size_t size = 5;
    /* How much is the buffer filled? */
    size_t read_size = 0;
    /* firs allocation, its result should be tested... */
    char *line = malloc(size);

    if (!line) {
        perror("malloc");
        return line;
    }

    while ((c = fgetc(stdin)) != EOF && c != '\n') {

        line[read_size++] = c;
        if (read_size == size) {
            size = read_size + size;
            char *test = realloc(line, size);
            if (!test) {
                perror("realloc");
                return line;
            }

            line = test;
        }
    }

    line[read_size] = '\0';

    return line;
}