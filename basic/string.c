#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  string s = get_string("Name of C50: ");
  if (strcmp(s, "Pedro") == 0) {
    s = "Pedro 2";
  }
  printf("Hello, %s\n", s);

  char full_name[] = {'Z', 'e', 'd', ' ', 'A', '.',
                      ' ', 'S', 'h', 'a', 'w', '\0'};

  printf("%s\n", full_name);

  char str[100];
  memset(str,'-', 100);

  printf("\n%s\n",str);

  return EXIT_SUCCESS;
}
