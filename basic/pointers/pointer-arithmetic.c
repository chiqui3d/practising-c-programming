#include <stdio.h>
#include <stdlib.h>

// https://aticleworld.com/pointer-arithmetic/

int main() {
    char *pcData = NULL;  // pointer to character
    float *pfData = NULL; // pointer to float
    printf("Address of character pointer before incrementation = %d\n\n\n", pcData);
    printf(" Address of float pointer before incrementation = %d\n\n\n", pfData);
    pcData++; // Increment the character pointer by one
    pfData++; // Increment the float pointer by one
    printf("Address of character pointer After incrementation = %d\n\n\n", pcData);
    printf("Address of float pointer After incrementation = %d\n\n\n", pfData);
    return 0;
}