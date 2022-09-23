#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// https://www.geeksforgeeks.org/how-arrays-are-passed-to-functions-in-cc/
// https://www.geeksforgeeks.org/g-fact/
// https://stackoverflow.com/questions/17590226/finding-length-of-array-inside-a-function
// https://stackoverflow.com/a/58630410/2046442
// https://stackoverflow.com/questions/13677890/how-to-check-if-a-string-is-in-an-array-of-strings-in-c

int main(void)
{

    int numbers[] = {4, 6, 8, 2, 7, 5, 0};
    int size = sizeof numbers / sizeof numbers[0];

    printf("int: \n");
    printf("Referencia: %p\n", &numbers);
    printf("Size %i\n", size);
    printf("int values: \n");
    for (int i = 0; i < size; i++)
    {
        printf("%i\n", numbers[i]);
    }

    char strings[] = "Hello, world";

    printf("strings: \n");
    printf("%s size: %li bytes and length: %lu \n", strings, sizeof(strings), strlen(strings));

    return EXIT_SUCCESS;
}
