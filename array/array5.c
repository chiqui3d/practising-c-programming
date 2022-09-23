#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int addElement(char ***arrayString, size_t newSize, char *newElement) 
{
   char **tmpArrayString = realloc(*arrayString, newSize * sizeof(char *)); 

  if (NULL == tmpArrayString) {
    perror("realloc() failed");
    free(arrayString);

    return EXIT_FAILURE;
  }

  *arrayString = tmpArrayString;
  (*arrayString)[newSize - 1] = newElement;

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

  size_t newSize = size + 1;

  addElement(&arrayString, newSize, "Hello, new world");

  for (int i = 0; i < newSize; i++) {
    printf("%s\n", arrayString[i]);
  }

  free(arrayString);

  return EXIT_SUCCESS;
}
