#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include  <math.h>


#define HASH_TABLE_SIZE 10

// Hash table with linked list

struct Node {
    int value;
    struct Node *next;
};

int functionHash(int value) {
    return value % HASH_TABLE_SIZE;
}


void insert(struct Node *hashTable, int value) {
    int index = functionHash(value);

    if (hashTable[index].value == -1) {
        hashTable[index].value = value;
        hashTable[index].next = NULL;
    } else {
        printf("Collision detected with the value %d and index %d, el cual ya tiene el valor %d\n", value, index, hashTable[index].value);
        // linked list
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->value = value;
        newNode->next = NULL;

        struct Node *last = hashTable[index].next;
        if (last == NULL) {
            hashTable[index].next = newNode;
        } else {
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = newNode;
        }

        printf("The value %d was inserted in the linked list of index %d\n", value, index);
    }
}

void delete (struct Node *hashTable, int value) {
    int index = functionHash(value);

    if (hashTable[index].value == value) {
        hashTable[index].value = -1;
        struct Node *first = hashTable[index].next;
        if (first != NULL) {
            hashTable[index].value = first->value;
            hashTable[index].next = first->next;
            free(first);
        }
    } else {
        // linked list
        struct Node *last = hashTable[index].next; 
        struct Node *previous = NULL;
        while (last != NULL) {
            if (last->value == value) {
                if (previous == NULL) {
                    hashTable[index].next = last->next;
                } else {
                    previous->next = last->next;
                }
                free(last);
                break;
            }
            previous = last;
            last = last->next;
        }
    }
}

void printHashTable(struct Node *hashTable, bool showEmpty) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (showEmpty == true && hashTable[i].value == -1) {
            printf("Index %d: %d\n", i, hashTable[i].value);
        }
        if (hashTable[i].value != -1) {
            printf("Index %d: %d\n", i, hashTable[i].value);
            struct Node *node = hashTable[i].next;
            while (node != NULL) {
                printf("\tIndex %d: %d\n", i, node->value);
                node = node->next;
            }
        }
    }
}

void search(struct Node *hashTable, int value) {
    int index = functionHash(value);

    struct Node *node = &hashTable[index];

    while (node != NULL) {
        if (node->value == value) {
            printf("Search: The value %d is in the hash table in the index %d of linked list\n", value, index);
            return;
        }
        node = node->next;
    }

    printf("Search: The value %d is not in the hash table\n", value);
}

int main(void) {

    struct Node hashTable[HASH_TABLE_SIZE];

    // initialize the hash table with -1
    memset(&hashTable, -1, sizeof(struct Node) * HASH_TABLE_SIZE);
    
    insert(hashTable, 10); // 0
    insert(hashTable, 15); // 5
    insert(hashTable, 40); // 0 => collision index, then linked list with next node
    insert(hashTable, 2);  // 2
    insert(hashTable, 60); // 0 => collision index, then linked list with next node

    delete (hashTable, 60);

    search(hashTable, 15);
    search(hashTable, 40);
    search(hashTable, 90);

    printHashTable(hashTable, false);

    // printf("Value of \n", hashTable[])

    printf("\n");

    return EXIT_SUCCESS;
}