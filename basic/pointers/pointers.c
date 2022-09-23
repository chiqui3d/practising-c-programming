// Stores and prints an integer via its address

#include <stdio.h>


int main(void)
{
    int n = 50;
    int *p = &n;
    printf("%i\n", n);
    printf("%i\n", *p);
    printf("address p: %p\n", p); // %p => hex memory address

    int newPtrValue = 60;
    int *ptr = NULL;
    ptr = &newPtrValue;
    printf("%i\n", *ptr);

    newPtrValue = 75;
    printf("%i\n", *ptr);

    *ptr = 85;
    printf("%i\n", *ptr);
    printf("%i\n", newPtrValue);

    *ptr = 90;
    printf("%i\n", *ptr);
    printf("%i\n", newPtrValue);

    char *s = "HI!";
    printf("%s\n", s); // string
    printf("%c\n", *s); // first
    printf("Reference");
    printf("%p\n", s);  // first memory address
    printf("%p\n", &s[0]); // first memory address
    printf("%p\n", &s[1]);
    printf("%p\n", &s[2]);
    printf("%p\n", &s[3]);


     // sugar synthetic
    printf("%c\n", *s);
    printf("%c\n", *(s + 1));
    printf("%c\n", *(s + 2));

    int numbers[] = {4, 6, 8, 2, 7, 5, 0};

    printf("%i\n", *numbers);
    printf("%i\n", *(numbers + 1));
    printf("%i\n", *(numbers + 2));
    printf("%i\n", *(numbers + 3));
    printf("%i\n", *(numbers + 4));
    printf("%i\n", *(numbers + 5));
    printf("%i\n", *(numbers + 6));
}
