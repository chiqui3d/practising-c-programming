
#include <stdio.h>  // for fprintf()
#include <stdlib.h> // for exit()
#include <string.h> // for strlen()
#include <ctype.h> // for tolower()
#include <limits.h> // for CHAR_MAX


// https://www.reddit.com/r/C_Programming/comments/jjdl7s/difference_between_char_and_char/
// https://www.cs.uic.edu/~jbell/CourseNotes/C_Programming/CharacterStrings.html
// https://stackoverflow.com/questions/9460260/what-is-the-difference-between-char-a-string-and-char-p-string/9631091#9631091
// https://www.reddit.com/r/C_Programming/comments/rxqp8u/when_to_use_char_array_vs_char_pointer/
// https://stackoverflow.com/questions/1335786/c-differences-between-char-pointer-and-array
// https://www.quora.com/What-is-the-difference-between-an-array-of-char-and-a-string-in-C
// https://aticleworld.com/what-is-the-difference-between-char-array-and-char-pointer-in-c/
// https://www.geeksforgeeks.org/whats-difference-between-char-s-and-char-s-in-c/
// https://stackoverflow.com/questions/12795850/string-literals-pointer-vs-char-array
// https://stackoverflow.com/questions/2036096/literal-string-initializer-for-a-character-array

/* 
char *concat(char *s1, char *s2) {
   char *catString = "Concatenated";

    while (**string2 != '\0') {
        printf("%c\n", **string2);
        (*string2)++;
    }
    **string2 = ' ';
    (*string2)++;


    while (*catString != '\0') {
        **string2 = *catString;
        (*string2)++;
        catString++;
    }

    **string2 = '\0';

    printf("\n%s\n\n", *string2);
    exit(0);
} */

int main() {


    const char *string0 = NULL;
    char string1[] = "Hello World 1";

    string0 = "Hello World";

    printf("\n%s\n\n", string0);

    string0 = "Hello World Modified";

    printf("\n%s\n\n", string0);

    string0 = string1;

    printf("\n%s\n\n", string0);

    printf("%s, Len: %lu, Size with null terminated: %lu\n", string1, strlen(string1), sizeof(string1));

    char string1_copy[100];
    strcpy(string1_copy, "string1_copy");
    printf("%s\n", string1);
    printf("%s\n\n", string1_copy);

    char *string2 = "Hello World 2";

    printf("Len: %lu, Size pointer: %lu, Size first char: %lu\n", strlen(string2), sizeof(string2), sizeof(*string2));

    char *string2_copy = strdup(string2);

    printf("%s\n", string2);
    printf("%s\n\n", string2_copy);
    free(string2_copy);

    char *string3 = "Hello World 3";
    char *string3_copy = strndup(string3, strlen(string3));

    printf("%s\n", string3);
    printf("%s\n\n", string3_copy);
    free(string3_copy);

    char *string4 = "Hello World 4";
    char string4_copy[strlen(string4) + 1];
    char string41_copy[strlen(string4) + 1];

    sprintf(string41_copy, "%s", string4);
    snprintf(string4_copy, strlen(string4) + 1, "%s", string4);

    printf("%s\n", string4);
    printf("%s\n", string4_copy);
    printf("%s\n\n", string41_copy);

    char *string5 = "Hello World 5";
    char string5_copy[strlen(string5) + 1];
    char *string51_copy = malloc(strlen(string5) + 1);

    memcpy(string5_copy, string5, strlen(string5) + 1);
    memcpy(string51_copy, string5, strlen(string5) + 1);

    string5_copy[0] = tolower(string5_copy[0]);
    //memset(string5_copy,tolower(string5_copy[0]),1); // alternative

    string51_copy = realloc(string51_copy, strlen(string5) + 2);
    strcat(string51_copy, "1");

    printf("%s\n", string5);
    printf("%s\n", string5_copy);
    printf("%s\n\n", string51_copy);
    free(string51_copy);


    char *string6 = "Hello World 6";
    char *string6_copy = malloc(strlen(string6) + 1);

    for (int i = 0; i < strlen(string6); i++) {
        string6_copy[i] = string6[i];
    }

    string6_copy[strlen(string6)] = '\0';
    printf("%s\n", string6);
    printf("%s\n\n", string6_copy);
    free(string6_copy);

    // String literals cannot be modified
    char *string7 = "Hello World 7";
    printf("%s\n\n", string7);

    struct String {
        const char *string1;
        char string2[CHAR_MAX];
        int length;
    };
    struct String string8;

    string8.string1 = "Hello World 8";
    string8.length = strlen(string8.string1);
    strcpy(string8.string2, string8.string1);
    printf("%s\n", string8.string1);
    printf("%s\n", string8.string2);
    printf("Len struct string8: %d\n\n", string8.length);

    struct String string9;

    //memset(&string9, 0, sizeof(string9));

    // An array name is not a modifiable lvalue. So, you cannot use the assignment operator (=) on that.
    // string9.string2 = "Hello World 9"; --> error
    strcpy(string9.string2, "Hello World 9");
    string9.length = strlen(string9.string2);
    string9.string1 = string9.string2;
    printf("%s\n", string9.string2);
    printf("%s\n", string9.string1);
    printf("Len struct string9: %d\n\n", string9.length);


    printf("Edit struct string9:\n\n");

    string9.string2[0] = tolower(string9.string2[0]);
    printf("%s\n\n", string9.string2);

    strcpy(string9.string2, "hello world 9");

    printf("%s\n\n", string9.string2);

    string9.string1 = "test char pointer";

    printf("%s\n", string9.string1);
    printf("Len string1: %lu\n\n", strlen(string9.string1));

     struct String string10;

    string10.string1 = "Hello World 10";
    string10.length = strlen(string10.string1);
    strcpy(string10.string2, string10.string1);
    printf("%s\n", string10.string1);
    printf("%s\n", string10.string2);
    printf("Len struct string10: %d\n\n", string10.length);

    // concatenation
    char *string11 = "Hello World 11";

    char *string12 = " Concatenation";

    char *tmp = malloc(strlen(string11) + strlen(string12) + 1);
    if (tmp == NULL) {
        printf("Error allocating memory\n");
        exit(1);
    }
    memcpy(tmp, string11, strlen(string11) + 1);
    strcat(tmp, string12);
    string11 = tmp;
    printf("string11: %s\n\n", string11);
    free(tmp);


    // concatenation with string literal
    char *string13 = "Hello World 13";
    char *string14 = " Concatenation";
    size_t len = strlen(string13) + strlen(string14) + 1;
    char tmp2[len];

    strcpy(tmp2, string13);
    strcat(tmp2, string14);

    string13 = tmp2;

    printf("%s\n", string13);




    return EXIT_SUCCESS;
}