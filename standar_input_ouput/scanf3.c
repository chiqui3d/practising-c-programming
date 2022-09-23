#include <stdio.h>
#include <stdlib.h>
#include "get_line.h"


int main(void) {

  static int rc;

  char name[10];

  rc = get_line("Enter name: ", name, sizeof(name));
  if (rc == NO_INPUT) {
    // Extra NL since my system doesn't output that on EOF.
    printf("\nNo input\n");
    return 1;
  }

  if (rc == TOO_LONG) {
    printf("Input too long: Max %s\n", name);
    return 1;
  }

  printf("Your name is %s\n", name);

  return 0;
}
