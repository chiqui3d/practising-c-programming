#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int number;
    struct node *next;
    struct node *prev;
};

struct DoubleLinkedList {
    struct node *head; // first node
    struct node *tail; // last node
};

struct DoubleLinkedList doubleList;

void pushNode(struct node **list, int number) {

    struct node *newNode = malloc(sizeof(struct node));
    newNode->number = number;
    newNode->next = *list;
    newNode->prev = NULL;

    if (*list != NULL) {
        (*list)->prev = newNode;
    }

    *list = newNode;
}

void appendNode(struct node **list, int number) {

    struct node *newNode = malloc(sizeof(struct node));
    newNode->number = number;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*list == NULL) {
        *list = newNode;
        doubleList.head = newNode;
    } else {
        struct node *last = *list;
        doubleList.head = *list;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newNode;
        newNode->prev = last;
        doubleList.tail = newNode;
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
    struct node *tmp = list;
    while (tmp != NULL) {
        if (tmp->number == number) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

struct node *searchNodePosition(struct node *list, int position) {
    struct node *tmp = list;
    int positionCounter = 1;

    for (; tmp != NULL && positionCounter < position; positionCounter++) {
        tmp = tmp->next;
    }
    if (tmp != NULL) {
        return tmp;
    }

    return NULL;
}

void removeNode(struct node **list, int number) {

    struct node *tmp = searchNode(*list, number);

    if (tmp == NULL || *list == NULL) {
        return;
    }

    /* If node to be deleted is head node */
    if (*list == tmp) {
        *list = tmp->next;
    }
    doubleList.head = *list;

    /* if is NOT the last node */
    if (tmp->next != NULL) {
        tmp->next->prev = tmp->prev;
    } else {
        doubleList.tail = tmp->prev;
    }

    /* if is NOT the first node */
    if (tmp->prev != NULL) {
        tmp->prev->next = tmp->next;
    }

    free(tmp);
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

// A utility function to find last node of linked list
struct node *lastNode(struct node *node) {
    while (node && node->next) {
        node = node->next;
    }

    return node;
}

int sizeOf(struct node *node) {
    int res = 0;
    while (node != NULL) {
        res++;
        node = node->next;
    }
    return res;
}

// This function prints contents of linked list starting
// from the given node
void printListReverse(struct node *node) {
    struct node *last;
    while (node != NULL) {
        last = node;
        node = node->next;
    }
    while (last != NULL) {
        printf("Number: %i\n", last->number);
        last = last->prev;
    }
    printf("\n");
}

void printListReverse2(struct node *node) {
    struct node *last = doubleList.tail;

    while (last != NULL) {
        printf("Number: %i\n", last->number);
        last = last->prev;
    }
    printf("\n");
}

void printFirstNode() {

    printf("First node: %i\n", doubleList.head->number);
}

void printLastNode() {

    printf("Last node: %i\n", doubleList.tail->number);
}

int main(void) {

    struct node *list = NULL;

    appendNode(&list, 1);
    appendNode(&list, 2);
    appendNode(&list, 3);
    appendNode(&list, 4);
    appendNode(&list, 5);
    insertAfter(list->next, 25);

    removeNode(&list, 3);

    printList(list);
    printf("Reverse list:\n");
    printListReverse(list);
    printListReverse2(list);

    printFirstNode();
    printLastNode();

    freeList(list);

    return EXIT_SUCCESS;
}
