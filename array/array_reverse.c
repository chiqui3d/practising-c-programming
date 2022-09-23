#include "../include/h_error.h"
#include <stdio.h>
#include <stdlib.h>

int *reverseArray(int array[], int size) {

    int i, temp;

    for (i = 0; i < size / 2; i++) {
        temp = array[i];
        array[i] = array[size - i - 1];
        array[size - i - 1] = temp;
    }

    return array;
}

int *reverseArray2(int *numbers, size_t size) {
    int *target = numbers;
    int i;
    for (i = 0; i < size; i++) {
        target[i] = numbers[size - i - 1];
    }
    return target;
}

int *reverseArray3(int arr[], int size) {
    int start = 0;
    int end = size - 1;
    int temp;
    while (start < end) {
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }

    return arr;
}

int main(void) {

    int numbers[] = {4, 6, 8, 2, 7, 5, 0};
    int size = sizeof numbers / sizeof numbers[0];

    printf("Array Size %i\n", size);

    for (int i = 0; i < size; i++) {
        printf("%i", numbers[i]);
        if (i != size - 1) {
            printf(",");
        }
    }
    printf("\n");

    int *newNumbers = reverseArray(numbers, size);

    for (int i = 0; i < size; i++) {
        printf("%i", newNumbers[i]);
        if (i != size - 1) {
            printf(",");
        }
    }

    printf("\n");

    return 0;
}
