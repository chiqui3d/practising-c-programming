#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * The last node of the list contains the address of the first node of the list.
 * The first node of the list also contain address of the last node in its previous pointer.
 */
struct node {
    int number;
    struct node *next;
    struct node *prev;
};

void pushNode(struct node **list, int number) {

    struct node *newNode = malloc(sizeof(struct node));
    newNode->number = number;

    if (*list == NULL) {
        *list = newNode;
        newNode->next = *list;
        newNode->prev = *list;
        return;
    }

    struct node *lastNode = *list;

    while (lastNode->next != *list) { // find the last node
        lastNode = lastNode->next;
    }
    lastNode->next = newNode; // last node points to new node, which is the first node
    newNode->prev = lastNode; // new node points to last node
    (*list)->prev = newNode;  // first node points to new node
    newNode->next = *list;    // new node points to first node
    *list = newNode;          // new node is the first node
}

void appendNode(struct node **list, int number) {

    struct node *newNode = malloc(sizeof(struct node));
    newNode->number = number;

    if (*list == NULL) {
        *list = newNode;
        newNode->next = *list;
        newNode->prev = *list;
    } else {
        struct node *last = *list;
        while (last->next != *list) {
            last = last->next;
        }
        last->next = newNode;
        newNode->prev = last;
        (*list)->prev = newNode;
        newNode->next = *list;
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
    newNode->prev = prevNode;

    if (newNode->next != NULL) {
        newNode->next->prev = newNode;
    }
}

struct node *searchNode(struct node *list, int number) {

    if (list == NULL) {
        return NULL;
    }
    // the first node
    if (list->number == number) {
        return list;
    }
    // the last node
    if (list->prev->number == number) {
        return list->prev;
    }

    struct node *tmp = list->next;
    do {
        if (tmp->number == number) {
            return tmp;
        }
        tmp = tmp->next;
    } while (tmp != list->prev);

    return NULL;
}

void removeNode(struct node **list, struct node *tmp) {

    if (tmp == NULL || *list == NULL) {
        return;
    }

    /* If node to be deleted is head node */
    if (*list == tmp) {
        *list = tmp->next;
    }

    /* if is NOT the last node */
    if (tmp->next != NULL) {
        tmp->next->prev = tmp->prev;
    }

    /* if is NOT the first node */
    if (tmp->prev != NULL) {
        tmp->prev->next = tmp->next;
    }

    free(tmp);
}

void removeByData(struct node **list, int number) {

    struct node *tmp = searchNode(*list, number);
    removeNode(list, tmp);
}

void removeFirstNode(struct node **list) {
    removeNode(list, *list);
}

void removeLastNode(struct node **list) {
    removeNode(list, (*list)->prev);
}

void printListReverse(struct node *node) {
    struct node *last = (*node).prev;

    do {
        printf("Number: %i\n", last->number);
        last = last->prev;
    } while (last != node->prev);

    printf("\n");
}

void freeList(struct node *node) {

    struct node *p = node;
    struct node *tmp;
    do {
        tmp = p;
        p = p->next;
        free(tmp);

    } while (node != p->next);

    free(p);
}

void printList(struct node *node) {
    struct node *tmp = node;
    if (node == NULL) {
        printf("List is empty\n");
        return;
    }

    do {
        printf("Number: %i\n", tmp->number);
        tmp = tmp->next;
    } while (node->next != tmp->next);
}

int main(void) {

    struct node *list = NULL;

    appendNode(&list, 1);
    appendNode(&list, 2);
    appendNode(&list, 3);
    appendNode(&list, 4);
    appendNode(&list, 5);

    struct node *tmp = searchNode(list, 2);

    if (tmp != NULL) {
        printf("Find %d\n", tmp->number);
    }
    pushNode(&list, 25);
    removeByData(&list, 25);

    printList(list);
    printf("Reverse list:\n");
    printListReverse(list);

    printf("Last node: %i\n", list->prev->number);

    freeList(list);

    return EXIT_SUCCESS;
}
