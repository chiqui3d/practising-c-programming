#include <stdio.h>
#include <stdlib.h>

int int_compare(const void *x, const void *y) {
  int val = *((int *)x) - *((int *)y);
  if (val < 0)
    return -1;
  else if (val > 0)
    return 1;
  else
    return 0;
}

int name_cmp(const void *s1, const void *s2) {
  return (((char *)s1)[0] - ((char *)s2)[0]);
}

int main(void) {
  int A[1000];

  for (int i = 0; i < 1000; i++)
  {
    A[i] = rand() % 1000;
  }
  qsort(A, 1000, sizeof(int), int_compare);

  for (int a = 0; a < 1000; a++) {
    printf("%i \n", A[a]);
  }

  char B[][10] = {"guna", "mccain", "obama", "paul", "barr"};
  qsort(*B, 5, 10, name_cmp);

  for (int j = 0; j < 5; j++) {
    printf("%s \n", B[j]);
  }

  return 0;
}
