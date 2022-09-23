#include <stdio.h>

// https://aticleworld.com/void-pointer-in-c/
int main(int argc, char *argv[]) {
    void *pvData = NULL; // void pointer
    int *iData = NULL;   // integer pointer
    char *cData = NULL;  // character pointer
    float *fData = NULL; // float pointer
    // size of void pointer
    printf("size of void pointer = %d\n\n", sizeof(pvData));
    // size of void pointer
    printf("size of integer pointer = %d\n\n", sizeof(iData));
    // size of void pointer
    printf("size of character pointer = %d\n\n", sizeof(cData));
    // size of void pointer
    printf("size of float pointer = %d\n\n", sizeof(fData));
    return 0;
}