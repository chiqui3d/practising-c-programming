#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define HASH_TABLE_SIZE 10

// Hash table with array

int functionHash(int value) {
    return value % HASH_TABLE_SIZE;
}

void insert(int *hashTable, int value) {
    int index = functionHash(value);

    if (hashTable[index] == -1) {
        hashTable[index] = value;
    } else {
        printf("Collision detected with the value %d and index %d, el cual ya tiene el valor %d\n", value, index, hashTable[index]);
        // exploración lineal
        int i = index + 1;
        for (; i < HASH_TABLE_SIZE; i++) {
            if (hashTable[i] == -1) {
                hashTable[i] = value;
                break;
            }
        }
        hashTable[i] = value;
        printf("The value %d was inserted in the index %d\n", value, i);
    }
}

void delete (int *hashTable, int value) {
    int index = functionHash(value);

    if (hashTable[index] == value) {
        hashTable[index] = -1;
    } else {
        // exploración lineal
        int i = index + 1;
        for (; i < HASH_TABLE_SIZE; i++) {
            if (hashTable[i] == value) {
                hashTable[i] = -1;
                break;
            }
        }
    }
}

void printHashTable(int *hashTable, bool showEmpty) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (showEmpty == true && hashTable[i] == -1) {
            printf("Index %d: %d\n", i, hashTable[i]);
        }
        if (hashTable[i] != -1) {
            printf("Index %d: %d\n", i, hashTable[i]);
        }
    }
}

void search(int *hashTable, int value) {
    int index = functionHash(value);

    if (hashTable[index] == value) {
        printf("The value %d is in the hash table in the index %d\n", value, index);
    } else {
        // exploración lineal
        int i = index + 1;
        for (; i < HASH_TABLE_SIZE; i++) {
            if (hashTable[i] == value) {
                break;
            }
        }
        printf("The value %d is in the hash table in the index %d\n", value, i);
    }
}

int main(void) {

    int hashTable[HASH_TABLE_SIZE];

    // initialize the hash table with -1
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable[i] = -1;
    }

    insert(hashTable, 10); // 0
    insert(hashTable, 15); // 5
    insert(hashTable, 40); // 0 => collision index + 1 = 1, es la primera posición vacía
    insert(hashTable, 2);  // 2

    search(hashTable, 15);
    search(hashTable, 40);

    printHashTable(hashTable, true);

    // printf("Value of \n", hashTable[])

    printf("\n");

    return EXIT_SUCCESS;
}