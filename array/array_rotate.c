#include <stdio.h>


/**
 * @brief Reverses the order of the elements in an array.
 * 
 * @param numbers The array of numbers.
 * @param size The size of the array.
 * @param rotate The number of elements to rotate.
 * 
 * @return int* The pointer to the array.
 */
int *swap(int numbers[], int size, int rotate) {

    for (int i = 0; i < rotate; i++) {
        int lastIndex = size - (i + 1);
        int tempRight = numbers[lastIndex];
        int tempLeft = numbers[i];
        numbers[i] = tempRight;
        numbers[lastIndex] = tempLeft;
    }

    return numbers;
}


/**
 * @tutorial https://www.geeksforgeeks.org/array-rotation/
 * 
 * @brief Rotates an array to the right by a given number of rotations
 * 
 * @param numbers The array to rotate.
 * @param size The size of the array.
 * @param rotations The number of rotations to rotate the array.
 * 
 * @return int* The rotated array. 
 */
int *rotateRight(int numbers[], int size, int rotations) {

    for (int i = 0; i < rotations; i++) {
        int j, last;
        // Stores the last element of the array
        last = numbers[size - 1];

        for (j = size - 1; j > 0; j--) {
            // Shift element of array by one
            numbers[j] = numbers[j - 1];
        }
        // Last element of array will be added to the start of array.
        numbers[0] = last;
    }

    return numbers;
}


/**
 * @tutorial https://www.geeksforgeeks.org/c-program-for-reversal-algorithm-for-array-rotation/
 * https://www.codingninjas.com/codestudio/library/reversal-algorithm-for-array-rotation
 * 
 * @example for one rotation
 * 
 * Input: [1, 2, 3, 4, 5, 6, 7]
 * Ouput: [2, 3, 4, 5, 6, 7, 1]
 * 
 * @brief Rotates an array to the left by a given number of rotations
 * 
 * @param numbers The array to rotate.
 * @param size The size of the array.
 * @param rotations The number of rotations to rotate the array.
 * 
 * @return int* The rotated array.
 * 
 */
int *rotateLeft(int numbers[], int size, int rotations) {

    int i, j;
    for (i = 0; i < rotations; i++) {
        int first = numbers[0];
        for (j = 1; j < size; j++) {
            numbers[j - 1] = numbers[j];
        }
        numbers[size - 1] = first;
    }

    return numbers;
}

int main(void) {

    int numbers[] = {1, 2, 3, 4, 5, 6, 7};
    int size = sizeof numbers / sizeof numbers[0];
    printf("Array Size %i\n", size);

    for (int i = 0; i < size; i++) {
        printf("%i", numbers[i]);
        if (i != size - 1) {
            printf(",");
        }
    }
    printf("\n");

    int *leftRotate = rotateLeft(numbers, size, 2);

    printf("Left:\n");
    for (int i = 0; i < size; i++) {
        printf("%i", leftRotate[i]);
        if (i != size - 1) {
            printf(",");
        }
    }
    printf("\n");

    int *rightRotate = rotateRight(numbers, size, 2);

    printf("Right:\n");
    for (int i = 0; i < size; i++) {
        printf("%i", rightRotate[i]);
        if (i != size - 1) {
            printf(",");
        }
    }
    printf("\n");

    return 0;
}
