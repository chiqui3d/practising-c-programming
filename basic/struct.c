// Demonstrates structs
#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int x, y;
} Point;

struct Foo {
  int x;
  int y;
};

int main(void)
{
 // allocate space for students
 Point point = {1, 4};
 //Point point = {.x = 1, .y = 5};

 struct Foo foo;
 foo.x = 2;
 foo.y = 5;

 printf("Point= x: %u, y: %u\n", point.x, point.y);
 printf("Foo= x: %u, y: %u\n", foo.x, foo.y);


}
