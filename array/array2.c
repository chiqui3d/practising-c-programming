#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://stackoverflow.com/questions/1088622/how-do-i-create-an-array-of-strings-in-c

int main(void) {

  char string[] = "Hello, One";
  char *arrayString[] = {"Hello, world", "Bye Bye, world"};
  size_t sizeArrayString = sizeof arrayString / sizeof arrayString[0];

  printf("%s\n", string);

  for (int i = 0; i < sizeArrayString; i++) {
    printf("%s\n", arrayString[i]);
  }

  return EXIT_SUCCESS;
}
