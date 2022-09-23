#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "h_string.h"
#include "../include/h_error.h"

char *str_repeat(const char *str, size_t count) {
  char *result = malloc(strlen(str) * count+1);

  if (NULL == result) {
    die("malloc() failed");
  }

  strcpy(result, str);
  for (size_t i = 1; i < count; i++) {
    // option 1 putchar('-');
    // option 2 result[i] = '-';
    // option 3
    strcat(result, str);
  }

  result[count] = '\0'; // end the null-byte

  return result;
}

