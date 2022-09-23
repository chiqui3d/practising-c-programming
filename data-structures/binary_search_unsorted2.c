// Search an element in an unsorted array using minimum number of comparisons

#include <stdio.h>
#include <stdlib.h>

int search(int arr[], int n, int x) {
    // 1st comparison
    if (arr[n - 1] == x) {
        return n - 1;
    }

    int backup = arr[n - 1];
    arr[n - 1] = x;
    // no termination condition and thus
    // no comparison
    for (int i = 0;; i++) {
        // this would be executed at-most n times
        // and therefore at-most n comparisons
        printf("%i\n", i);
        printf("%i==%i\n", arr[i], x);
        if (arr[i] == x) {

            // replace arr[n-1]
            // with its actual element
            // as in original 'arr[]'
            //arr[n - 1] = backup;
            
            // if 'x' is found before the '(n-1)th'
            // index, then it is present in the array
            // final comparison
            printf("%i < %i\n", i, n - 1);
            if (i < n - 1) {
                return i;
            }

            // else not present in the array
            return -1;
        }
    }
}

int main() {
    int arr[] = {15, 12, 13, 19, 11, 10, 18, 17, 14, 16, 40, 50, 60, 70, 80, 90};
    int number = 16;
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Find number: %i\n", number);
    printf("Array size: %i\n", size);

    int index = search(arr, size, number);
    if (index != -1) {
        printf("Found at index %i\n", index);
    } else {
        printf("Not found\n");
    }

    return 0;
}