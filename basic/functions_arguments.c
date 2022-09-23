#include <stdarg.h>
#include <stdio.h>

/* args supplies count of other args */
int sum(int n_args, ...) {

  int result = 0;
  va_list ap;
  va_start(ap, n_args);

  printf("Nº Arguments %i\n", n_args);

  for (int i = 0; i < n_args; i++) {
    result += va_arg(ap, int);
  }

  va_end(ap);

  return result;
}

int main(void) {
  printf("Sum: %d\n", sum(5, 1, 1, 10, 10, 1));

  return 0;
}
