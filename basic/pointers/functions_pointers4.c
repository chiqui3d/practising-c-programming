#include <stdio.h>
#include <stdlib.h>

typedef int (*fn)(void *, int);

int array_len(void *any, fn term)
{
  int length = 0;
  while ((*term)(any, length) == 0)
    length++;
  return length;
}

int int_func(int A[], int i){
 return (A[i] == -999);
}

int int_compare(const void *x, const void *y) {
  int val = *((int *)x) - *((int *)y);
  if (val < 0)
    return -1;
  else if (val > 0)
    return 1;
  else
    return 0;
}

#define update(arr,pos,val) do{arr[pos]=val;}while(0)


int main(void)
{

   int A[1000];

   for (int i = 0; i < 1000; i++)
   {
     A[i] = rand() % 1000;
   }

   A[999] = -999;

   //qsort(A, 1000, sizeof(int), int_compare);

   printf("Array Len: %d\n",array_len(A, (fn)int_func));

  return 0;
}
