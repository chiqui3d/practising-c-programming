// Capitalizes a copy of a string using strcpy

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  const char src[50] = "https://www.clicksolution.es";
  char dest[50];

  strcpy(dest, "Hello word!");
  printf("Before memcpy dest = %s\n", dest);

  memcpy(dest, src, strlen(src) + 1);
  printf("After memcpy dest = %s\n", dest);


  return EXIT_SUCCESS;
}
