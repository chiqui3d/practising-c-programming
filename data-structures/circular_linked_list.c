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

    if (*list == NULL) {
        *list = newNode;
        newNode->next = *list;
        return;
    }

    struct node *last = *list;

    while (last->next != *list) {
        last = last->next;
    }
    newNode->next = *list;
    *list = newNode;
    last->next = *list;
}

void appendNode(struct node **list, int number) {

    struct node *newNode = malloc(sizeof(struct node));
    newNode->number = number;
    newNode->next = NULL;

    if (*list == NULL) {
        *list = newNode;
        newNode->next = *list;

    } else {
        struct node *last = *list;
        do {
            last = last->next;
        } while (last->next != *list);

        newNode->next = *list;
        last->next = newNode;
    }
}

// A utility function to find last node of linked list
struct node *lastNode(struct node *node) {
    struct node *first = node;
    do {
        node = node->next;
    } while (node->next != first);

    return node;
}

// TODO: Fix this function
void removeNode(struct node **list, int number) {
    struct node *tmp = *list;
    struct node *prev = NULL;

    while (tmp != NULL) {
        if (tmp->number == number) {
            if (prev == NULL) {
                struct node *last = lastNode(*list);
                *list = tmp->next;
                last->next = *list;
                printf("Removed node with number %d\n", number);
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

void freeList(struct node *node) {

    struct node *first = node;
    struct node *tmp;
    do {
        tmp = node;
        node = node->next;
        free(tmp);

    } while (node != first);
}

void printList(struct node *node) {
    struct node *first = node;
    do {
        printf("Number: %i\n", node->number);
        node = node->next;
    } while (node != first);
}

int main(void) {

    struct node *list = NULL;

    appendNode(&list, 1);
    appendNode(&list, 2);
    appendNode(&list, 3);
    appendNode(&list, 4);
    appendNode(&list, 5);

    pushNode(&list, 25);

    removeNode(&list, 25);

    struct node *last = lastNode(list);
    printf("Last node is %i\n", last->number);

    printList(list);

    freeList(list);

    return EXIT_SUCCESS;
}
