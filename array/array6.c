#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int addElement(char ***arrayString, size_t size, char *newElement) {
  char **tmpArrayString = realloc(*arrayString, size * sizeof(char *));

  if (NULL == tmpArrayString) {
    perror("realloc() failed");
    free(arrayString);

    return EXIT_FAILURE;
  }

  *arrayString = tmpArrayString;
  (*arrayString)[size - 1] = newElement;

  return EXIT_SUCCESS;
}

int deleteElement(char ***arrayString, size_t size, int index) {

  int i;

  for (i = 0; i < size; i++) {
    if (i < index) {
      (*arrayString)[i] = (*arrayString)[i];
    } else if (i > index) {
      (*arrayString)[i - 1] = (*arrayString)[i];
    }
  }

  *arrayString = realloc(*arrayString, (size - 1) * sizeof(char *));

  return EXIT_SUCCESS;
}

int main(void) {

  size_t size = 2;
  char **arrayString = malloc(size * sizeof(char *));

  if (NULL == arrayString) {
    perror("malloc() failed");
    free(arrayString);

    return EXIT_FAILURE;
  }

  arrayString[0] = "Hello, world";
  arrayString[1] = "Bye Bye, world";

  size++;
  addElement(&arrayString, size, "Hello, new world");
  size++;
  addElement(&arrayString, size, "Goodnight, new world");
  deleteElement(&arrayString, size, 2);
  size--;

  arrayString[0] = "Update Hello";

  for (int i = 0; i < size; i++) {
    printf("%d: %s\n", i, arrayString[i]);
  }

  free(arrayString);

  return EXIT_SUCCESS;
}
