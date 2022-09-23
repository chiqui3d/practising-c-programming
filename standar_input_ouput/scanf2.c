// Gets an int from user using scanf

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char name[50];

    printf("What is your name: ");
    // Use & to get addresses of non-pointer variables. An array is, by definition, a pointer
    // (whose value is an address), so we use the array name itself (blah in the above example)
    // to input a string. (Equivalently, we could have used &blah[0].)
    scanf("%s", name);

    printf("name: %s\n", name);
}
