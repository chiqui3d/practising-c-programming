#include <stdio.h>
#include <stdlib.h>

int mystery(int a, int b, int (*fn)(int, int)) { 
  return ((*fn)(a, b)); 
}

int sum_of_squares(int x, int y) { 
  return (x * x + y * y); 
}

int sum(int x, int y) { 
  return (x + y); 
}

int main(void) {

  printf("%d \n", mystery(10, 12, sum_of_squares));
  printf("%d \n", mystery(10, 12, sum));

  return 0;
}
