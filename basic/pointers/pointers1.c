#include <stdio.h>
#include <stdlib.h>


// C passes arguments by value, not by reference

// https://stackoverflow.com/questions/24723423/different-pointer-address-printed-inside-function
// https://stackoverflow.com/questions/21524705/c-modify-the-address-of-a-pointer-passed-to-a-function
// https://www.reddit.com/r/C_Programming/comments/uhf9l4/where_does_this_memory_address_come_from/
// https://blog.feabhas.com/2010/09/scope-and-lifetime-of-variables-in-c/


// pointer address (copy in the stack). 
// Useful for change value. No change real pointer
void testPointer1(int *ptr) 
{
  
  (*ptr)++; // adding 1 to *ptr
  printf("Pointer Value: %i\n", *ptr);
  printf("Number Address: %p\n", ptr);
  printf("Pointer Copy, not real pointer address: %p\n\n", &ptr);

}

// pointer to a pointer (real address). 
// Useful for realloc() and change value. Change real pointer
void testPointer2(int **ptr) 
{ 
    (**ptr)++; // adding 1 to *ptr
    printf("Pointer Value: %i \n", **ptr);
    printf("Number Address: %p \n", *ptr);
    printf("Real Pointer Address, thanks pointer to pointer: %p \n", ptr);
    printf("Pointer Copy: %p \n\n", &ptr);
}

int main(void) {

  int a = 5;

  printf("a: %i \n", a);
  printf("a: %p \n", &a);

  int *p = &a;

  printf("p: %i \n", *p);
  printf("p: %p \n", &p);
  printf("p: %p \n", p);
  
  // change pointer value
  a = 3;

  printf("a: %i \n", *p);

  // -------------------------------------------------- //

  int *ptr;
  int number = 10;
  
  // assign address to the new pointer
  // &=address of ...
  ptr = &number; 

  // The * operator is also the dereference operator,
  // which goes to an address to get the value stored there
  printf("Pointer Value: %i\n", *ptr);
  printf("Number Address: %p\n", ptr);
  printf("Number Address: %p\n", &number);
  printf("Pointer Address: %p\n\n", &ptr);

  testPointer1(ptr);
  testPointer1(ptr);
  testPointer2(&ptr);
  testPointer2(&ptr);
  
  printf("New Number Value: %i\n", number);

  return EXIT_SUCCESS;
}
