#include <stdio.h>
#include <stdlib.h>

typedef int (*Ptr)(int,int);

int sum(int x, int y) { return (x + y); }
int difference(int x, int y) { return (x - y); }

Ptr Math(const char code)
{
 if (code == '+') return sum;

 return difference;

}


int main(void)
{
  Ptr ptr = Math('+');

  printf("%d \n", ptr(2,4));

  return 0;
}
