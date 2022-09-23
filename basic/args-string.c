
#include <stdio.h>  // for fprintf()
#include <stdlib.h> // for exit()
#include <string.h> // for strlen()


// https://flaviocopes.com/c-return-string/
// https://stackoverflow.com/a/9379119
// https://stackoverflow.com/a/8073585
// https://www.quora.com/How-can-I-pass-a-string-as-an-argument-to-a-function-and-return-a-string-in-C
// https://stackoverflow.com/questions/39310915/pass-char-as-reference-in-c


const char *fString0() {

    const char *string0 = "Hello World 0";

    return string0;
}

char *fString1(char *string1) {

    string1 = "Hello World 1 Renamed";

    printf("\n%s\n\n", string1);

    return string1;
}


char *fString11(char *string1) {

    strcpy(string1, "Hello World 11 Renamed");

    printf("\n%s\n\n", string1);

    return string1;
}

char *fString2(char **string2) {

    *string2 = "Hello World 2 Renamed";

    printf("\n%s\n\n", *string2);

    return *string2;
}

char *fString3(char string3[14]) {

    strcpy(string3, "Hello World 3 Renamed");

    printf("\n%s\n\n", string3);

    return string3;
}

char *fString5(char string5[]) {

    printf("Len: %lu\n\n", strlen(string5));

    strncpy(string5, "Hello World 6", strlen(string5) + 1);

    printf("%s\n\n", string5);

    return string5;
}

char *fString6(char *string6, size_t size) {

    strncpy(string6, "Hello World 7", size + 1);

    printf("%s\n\n", string6);

    return string6;
}

int sum()
{
    int a = 50, b = 80;
    int result = a + b;
    return result;
}

int main() {

    const char *string0 = fString0();
    printf("%s\n\n", string0);

    string0 = "Hello World 0 Modified";

    printf("%s\n\n", string0);

    char *string1 = "Hello World 1";
    printf("%s\n", string1);
    printf("%s\n\n", fString1(string1));
    printf("%s\n\n", string1);

    char string11[100];

    printf("The best way:\n");
    printf("STRING 11: %s\n", fString11(string11));
    printf("STRING 11: %s\n\n", string11);

    char *string2 = "Hello World 2";
    printf("%s\n", string2);
    printf("%s\n\n", fString2(&string2));
    printf("%s\n\n", string2);

    char string3[14] = "Hello World 3";
    printf("%s\n", string3);
    printf("%s\n\n", fString3(string3));
    printf("%s\n\n", string3);

    /**
     * BAD
     * This is rare, with 2 bytes size, could copy 14 bytes with strcpy
     * https://stackoverflow.com/questions/65378690/how-can-the-strcpy-function-copy-a-large-string-into-a-smaller-string
     *
     */
    char string4[2];

    strcpy(string4, "Hello World 4");

    printf("%s\n\n", string4);

    printf("Len: %lu, Size pointer: %lu\n\n", strlen(string4), sizeof(string4));

    /** end bad */

    char string5[] = "Hello World 5";
    printf("%s\n", string5);
    printf("%s\n\n", fString5(string5));
    printf("%s\n\n", string5);

    char string6[] = "Hello World 6";
    printf("%s\n", string6);
    printf("%s\n\n", fString6(string6, strlen(string6)));
    printf("%s\n\n", string6);

    int num = sum();
    printf("\nSum of two given values = %d\n", num);

    return EXIT_SUCCESS;
}