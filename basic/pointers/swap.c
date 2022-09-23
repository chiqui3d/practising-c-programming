#include <stdio.h>
#include <assert.h>

void swap(int *a, int *b);

int main(void)
{
    int a = 1;
    int b = 2;

    //assert(a == b);

    printf("a: %i, and b: %i \n", a, b);
    swap(&a,&b);
    printf("a: %i, and b: %i \n", a, b);

    return 0;
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
