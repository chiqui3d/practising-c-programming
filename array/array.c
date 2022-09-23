#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  int numbers[] = {4, 6, 8, 2, 7, 5, 0};
  int size = sizeof numbers / sizeof numbers[0];

  printf("Referencia: %p\n", &numbers);
  printf("Array Size %i\n", size);

  for (int i = 0; i < size; i++) {
    printf("%i\n", numbers[i]);
  }

  int arrayNumbers[20];
  memset(arrayNumbers, -1, sizeof(arrayNumbers));

  arrayNumbers[1] = 20;
  arrayNumbers[10] = 10;

  printf("Start\n");
  for (int i = 0; i < 20; i++) {
    printf("%i\n", arrayNumbers[i]);
  }

  return 0;
}
