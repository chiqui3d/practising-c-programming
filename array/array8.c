#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void push_char(char *newElement, char ***arrayString, size_t *size);
void push_int(int newElement, int **arrayInt, size_t *size);
char *str_repeat(const char *str, size_t count);

#define push(element, array, size)                                             \
  _Generic((element), \
  char *: push_char, \
  int: push_int,  \
  default: push_char) (element,array, size)

int main(void) {

  size_t sizeArrayString = 2;

  char **arrayString = malloc(sizeArrayString * sizeof(char *));

  if (NULL == arrayString) {
    perror("malloc() failed");
    free(arrayString);

    return EXIT_FAILURE;
  }

  arrayString[0] = "Hello, world";
  arrayString[1] = "Hello, C world!";

  push("Bye Bye, world", &arrayString, &sizeArrayString);
  push("Goodnight, world", &arrayString, &sizeArrayString);

  for (int i = 0; i < sizeArrayString; i++) {
    printf("%i: %s\n", i, arrayString[i]);
  }
  free(arrayString);

  char *separator = str_repeat("-", 30);
  printf("%s\n", separator);
  free(separator);

  size_t sizeArrayInt = 2;
  int *arrayInt = malloc(sizeArrayInt * sizeof(int));

  arrayInt[0] = 4;
  arrayInt[1] = 5;

  push(1, &arrayInt, &sizeArrayInt);

  for (int i = 0; i < sizeArrayInt; i++) {
    printf("%i: %i\n", i, arrayInt[i]);
  }

  free(arrayInt);

  return EXIT_SUCCESS;
}

/**
 * It adds a new element to the end of the array
 *
 * @param newElement The new element to add.
 * @param arrayString a pointer to a pointer to a pointer to a char.
 * @param size The number of strings in the array.
 *
 * @return void
 */
void push_char(char *newElement, char ***arrayString, size_t *size) {
  (*size)++;

  char **tmpArrayString = realloc(*arrayString, (*size) * sizeof(char *));

  if (NULL == tmpArrayString) {
    perror("realloc() failed");
    free(arrayString);

    exit(EXIT_FAILURE);
  }

  *arrayString = tmpArrayString;
  (*arrayString)[(*size) - 1] = newElement;
}

void push_int(int newElement, int **arrayInt, size_t *size) {
  (*size)++;

  int *tmpArrayInt = realloc(*arrayInt, (*size) * sizeof(int));

  if (NULL == tmpArrayInt) {
    perror("realloc() failed");
    free(arrayInt);

    exit(EXIT_FAILURE);
  }

  *arrayInt = tmpArrayInt;
  (*arrayInt)[(*size) - 1] = newElement;
}

char *str_repeat(const char *str, size_t count) {
  char *result = malloc(strlen(str) * count + 1);

  if (NULL == result) {
    perror("malloc() failed");
    free(result);

    exit(EXIT_FAILURE);
  }

  strcpy(result, str); 
  for (size_t i = 1; i < count; i++) {
    // option 1 putchar('-');
    // option 2 result[i] = '-';
    // option 3
    strcat(result, str);
  }

  result[count] = '\0'; // setting the null-byte
 

  return result;
}