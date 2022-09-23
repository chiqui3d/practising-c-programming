// Gets an int from user using scanf

#include <stdio.h>

int main(void)
{
    // https://stackoverflow.com/questions/1247989/how-do-you-allow-spaces-to-be-entered-using-scanf
    int x;
    printf("x: ");
    scanf("%i", &x);
    printf("x: %i\n", x);
}
