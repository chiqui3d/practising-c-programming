// Implements binary search for sorted numbers
#include <stdio.h>
#include <stdlib.h>

int binarySearch(int numbers[], int size, int search);
int recursiveBinarySearch(int numbers[], int start_index, int end_index, int search);
int compare(const void *m, const void *n) { return (*(int *)m - *(int *)n); }

int main(void) {
    // An array of numbers
    int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 40, 50, 60, 70, 80, 90};
    int length = sizeof(numbers) / sizeof(int);
    int number = 90;
    int index = binarySearch(numbers, length, number);
    if (index != -1) {
        printf("%i found at index %i\n", number, index);
    } else {
        printf("%i not found\n", number);
    }

    int number2 = 8;
    int index2 = recursiveBinarySearch(numbers, 0, length - 1, number2);
    if (index2 != -1) {
        printf("%i found at index2 %i\n", number2, index2);
    } else {
        printf("%i not found\n", number2);
    }

    int number3 = 3;
    int *index3 = bsearch(&number3, numbers, length, sizeof(int), compare);

    if (index3 != NULL) {
        printf("%i found at index3 %i\n", number3, *index3);
    } else {
        printf("%i not found\n", number3);
    }

    return 0;
}

int recursiveBinarySearch(int numbers[], int start_index, int end_index, int search) {
    if (end_index >= start_index) {
        int middle = start_index + (end_index - start_index) / 2;
        if (numbers[middle] == search) {
            return middle;
        }

        if (search > numbers[middle]) { // number is on the right side
            return recursiveBinarySearch(numbers, middle + 1, end_index, search);
        }
        // number is on the left side
        return recursiveBinarySearch(numbers, start_index, middle - 1, search);
    }
    return -1;
}

int binarySearch(int numbers[], int size, int search) {
    int low = 0;
    int high = size - 1;
    int mid;
    if (low > high) {
        return -1;
    }

    while (low <= high) {
        // https://www.techiedelight.com/binary-search/#Overflow
        mid = (low + high) / 2;
        printf("mid: %i\n", mid);
        if (numbers[mid] == search) {
            return mid;
        } else if (search > numbers[mid]) { // number is on the right side
            low = mid + 1;
            printf("low: %i\n", low);
        } else { // number is on the left side
            high = mid - 1;
            printf("high: %i\n", high);
        }
    }

    return -1;
}