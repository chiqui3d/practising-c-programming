#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int number;
    struct node *next;
};

void pushNode(struct node **list, int number) {

    struct node *newNode = malloc(sizeof(struct node));
    newNode->number = number;
    newNode->next = *list;

   *list = newNode;
}

void appendNode(struct node **list, int number) {

    struct node *newNode = malloc(sizeof(struct node));
    newNode->number = number;
    newNode->next = NULL;

    if (*list == NULL) {
        *list = newNode;
    } else {
        struct node *last = *list;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newNode;
    }
}

void insertAfter(struct node *prevNode, int number) {

    if (prevNode == NULL) {
        printf("The given previous node cannot be NULL");
        return;
    }

    struct node *newNode = malloc(sizeof(struct node));
    newNode->number = number;
    newNode->next = prevNode->next;
    prevNode->next = newNode;
}

struct node *searchNode(struct node *list, int number) {
    struct node *tmp = list;
    while (tmp != NULL) {
        if (tmp->number == number) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

void removeNode(struct node **list, int number) {
    struct node *tmp = *list;
    struct node *prev = NULL;
    while (tmp != NULL) {
        if (tmp->number == number) {
            if (prev == NULL) {  // first node
                *list = tmp->next;
            } else {
                prev->next = tmp->next;
            }
            free(tmp);
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

// A utility function to find last node of linked list
struct Node *lastNode(struct node *node) {
    while (node && node->next) {
        node = node->next;
    }

    return node;
}

void freeList(struct node *node) {
    struct node *tmp;
    while (node != NULL) {
        tmp = node;
        node = node->next;
        free(tmp);
    }
}

void printList(struct node *node) {
    while (node != NULL) {
        printf("Number: %i\n", node->number);
        node = node->next;
    }
}

int main(void) {

    struct node *list = NULL;

    appendNode(&list, 1);
    appendNode(&list, 2);
    appendNode(&list, 3);
    appendNode(&list, 4);
    appendNode(&list, 5);
    insertAfter(list->next->next->next, 25);

    removeNode(&list, 3);

    printList(list);

    freeList(list);

    return EXIT_SUCCESS;
}
