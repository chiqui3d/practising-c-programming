#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/h_error.h"

typedef int (*compare_int)(int a, int b);

int *bubbleSort(int *numbers, size_t size, compare_int cmp);

int sort_asc(int a, int b);
int sort_desc(int a, int b);

int main(void) {

    int integers[] = {5, 8, 9, 20, 1, 2, -20};

    size_t size = sizeof integers / sizeof integers[0];

    int *orderedAsc = bubbleSort(integers, size, sort_asc);

    size_t i;
    for (i = 0; i < size; i++) {

        printf("%i\t", orderedAsc[i]);
    }

    printf("\n");

    free(orderedAsc);

    // ------------------------ //

    int *orderedDesc = bubbleSort(integers, size, sort_desc);

    for (i = 0; i < size; i++) {

        printf("%i\t", orderedDesc[i]);
    }

    printf("\n");

    free(orderedDesc);

    return EXIT_SUCCESS;
}

int sort_asc(int a, int b) { return a - b; }

int sort_desc(int a, int b) { return b - a; }

int *bubbleSort(int *numbers, size_t size, compare_int cmp) {

    size_t i, j, tmp;

    int *target = malloc(size * sizeof(int));

    if (NULL == target) {
        die("target malloc");
    }

    memcpy(target, numbers, size * sizeof(int));

    for (i = 0; i < size; i++) {
        for (j = 0; j < size - 1; j++) {
            if (cmp(target[j], target[j + 1]) > 0) {
                tmp = target[j + 1];
                target[j + 1] = target[j];
                target[j] = tmp;
            }
        }
    }

    return target;
}