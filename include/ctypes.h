#include <string.h>
#include <stdbool.h>

/**
 * @link https://stackoverflow.com/questions/6280055/how-do-i-check-if-a-variable-is-of-a-certain-type-compare-two-types-in-c
 * @link http://www.robertgamble.net/2012/01/c11-generic-selections.html
 * @link https://stackoverflow.com/questions/17667189/create-a-c-function-that-accepts-parameters-of-different-data-types
 */
#define typecheck(type, x) _Generic(x, type: true, default: false)
#define typename(x) _Generic((x), \
    _Bool: "_Bool", \
    char: "char", \
    int: "int", \
    float: "float", \
    double: "double", \
    long double: "long double", \
    signed char: "signed char", \
    unsigned char: "unsigned char", \
    short int: "short int", \
    unsigned short int: "unsigned short int", \
    unsigned int: "unsigned int", \
    long int: "long int", \
    unsigned long int: "unsigned long int", \
    long long int: "long long int", \
    unsigned long long int: "unsigned long long int", \
    char *: "pointer to char", \
    void *: "pointer to void", \
    int *: "pointer to int", \
    default: "other")