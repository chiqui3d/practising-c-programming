#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include  <math.h>


#define HASH_TABLE_SIZE 10

// Hash table with linked list
struct Node {
    char name[20];
    int phone;
    struct Node *next;
};

// convert char array to int
unsigned int functionHash(char *str) {
    unsigned int value = 0;

    int i = 0;
    for (; i < strlen(str); i++) {
        value += str[i];
    }
    return value % HASH_TABLE_SIZE;
}

unsigned int functionHashLength(char *str) {
    return strlen(str) % HASH_TABLE_SIZE;
}

unsigned int countDigits(int number) {
    return floor(log10(number)) + 1;
}

unsigned int functionHashMidSquare(int value) {
    
    unsigned int square = value * value;
    unsigned short int digits = countDigits(square);
    unsigned short int mid = fabs(ceil(digits / 2));
    unsigned short int needDigits = 2;
    if (digits % 2 != 0) { // left position mid when is odd
        mid = mid - 1;
    }

    // convert int to char array
    char str[digits+1];
    snprintf(str, digits+1, "%d", square);

    // get the middle digits
    char strMidSquareNumber[needDigits+1];
    unsigned short int i = 0;
    for (; i < needDigits; i++) {
        strMidSquareNumber[i] = str[mid + i];
    }
    strMidSquareNumber[needDigits] = '\0';
    // convert char array to int
    unsigned short int midSquareNumber = atoi(strMidSquareNumber);
    printf("midSquareNumber of %d: %d\n",square, midSquareNumber);

    return square % HASH_TABLE_SIZE;
}

void initHasTable(struct Node *hasTable[]){
    int i = 0;
    for (; i < HASH_TABLE_SIZE; i++) {
        hasTable[i] = NULL;
    }
}

void insert(struct Node *hashTable, char *name, int phone) {
    unsigned int index = functionHash(name);

    if (hashTable[index].name[0] == '\0') {
        strcpy(hashTable[index].name, name);
        hashTable[index].phone = phone;
        hashTable[index].next = NULL;
    } else {
        // linked list
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        strcpy(newNode->name, name);
        newNode->phone = phone;
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
        printf("Collision detected with the name %s and index %d, which already has the value %s," 
               " insert in the *next pointer.\n", name, index, hashTable[index].name);
    }
}

void delete (struct Node *hashTable, char *name) {
    unsigned int index = functionHash(name);

    if (strcmp(hashTable[index].name, name) == 0) {
        strcpy(hashTable[index].name, "");
        struct Node *first = hashTable[index].next;
        if (first != NULL) {
            strcpy(hashTable[index].name, first->name);
            hashTable[index].next = first->next;
            free(first);
        }
    } else {
        // linked list
        struct Node *last = hashTable[index].next; 
        struct Node *previous = NULL;
        while (last != NULL) {
            if (strcmp(last->name, name) == 0) {
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


void search(struct Node *hashTable, char *name) {
    unsigned int index = functionHash(name);

    struct Node *node = &hashTable[index];

    while (node != NULL) {
        if (strcmp(node->name, name) == 0) {
            printf("Search: The name %s is in the hash table in the index %d of linked list\n", node->name, index);
            return;
        }
        node = node->next;
    }

    printf("Search: The name %s is not in the hash table\n", name);
}

void printHashTable(struct Node *hashTable, bool showEmpty) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (showEmpty == true && hashTable[i].name[0] == '\0') {
            printf("Index %d: %s\n", i, hashTable[i].name);
        }
        if (hashTable[i].name[0] != '\0') {
            printf("Index %d: %s, phone: %d\n", i, hashTable[i].name, hashTable[i].phone);
            struct Node *node = hashTable[i].next;
            while (node != NULL) {
                printf("\t %s, phone: %d\n", node->name, node->phone);
                node = node->next;
            }
        }
    }
}

int main(void) {

    struct Node hashTable[HASH_TABLE_SIZE];
    memset(hashTable, 0, sizeof(hashTable));

    char *agenda[] = {"Alberto","Manolo", "Antonio" , "Roberto", "Jose Carlos"};
    int phone[] = {123456789, 987654321, 123456789, 987654321, 123456789};
    int agendaSize = sizeof agenda / sizeof agenda[0];

    printf("Mid square of 123 function: %d\n", functionHashMidSquare(123));

    for (int i = 0; i < agendaSize; i++) {
        insert(hashTable, agenda[i], phone[i]);
    }

    search(hashTable, "Alberto");

    delete(hashTable, "Manolo");

    printHashTable(hashTable, false);
    // freeHashTable(hashTable);
    printf("\n");

    return EXIT_SUCCESS;
}