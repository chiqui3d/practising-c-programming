#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

#include "h_error.h"

void die(const char *message, ...) {

  va_list args;
  va_start(args, message);

  if (errno) {
    char buffer[CHAR_MAX];
    vsprintf(buffer, message, args);
    perror(message);
  } else {
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
  }
  va_end(args);

  exit(EXIT_FAILURE);
}