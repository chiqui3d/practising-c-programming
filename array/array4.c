#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://www.reddit.com/r/C_Programming/comments/ugac6s/realloc_invalid_pointer_when_i_add_a_new_element/

char** addElement(char **arrayString, size_t newSize, char *newElement) 
{
  char **tmpArrayString = realloc(arrayString, newSize * sizeof(char *));

  arrayString = tmpArrayString;
  arrayString[newSize - 1] = newElement;

  return arrayString;

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

  size_t newSize = size + 1;

  arrayString = addElement(arrayString, newSize, "Hello, new world");

  for (int i = 0; i < newSize; i++) {
    printf("%s\n", arrayString[i]);
  }

  free(arrayString);

  return EXIT_SUCCESS;
}
