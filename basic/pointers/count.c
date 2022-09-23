#include <stdio.h>
#include <assert.h>

/* If we're not using GNU C, elide __attribute__ */
#ifndef __GNUC__
#  define  __attribute__(x)  /*NOTHING*/
#endif

// extern void foo(int *count) __attribute__((noreturn));

void foo(int *count);

int main(void)
{
    int count = 5;

    assert(count == 5);

    printf("count: %i and address: %p \n",count, &count);
    foo(&count);
    printf("count: %i and address: %p \n",count, &count);

     return 0;
}

// [[noreturn]]
void foo(int *count)
{
    ++*count;
}
