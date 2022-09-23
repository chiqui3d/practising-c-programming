#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_SIZE 5
int currentSize = -1; // because we start from 0

// Check if the queue is empty
bool isEmpty() {
    if (currentSize == -1) {
        return true;
    }
    return false;
}
// Check if the queue is full
bool isFull() {
    if (currentSize == QUEUE_SIZE - 1) {
        return true;
    }
    return false;
}
// Get the value of the front of the queue without removing it
int peek(int *queue) {
    if (isEmpty()) {
        printf("Queue is empty\n");
        return -1;
    }
    return queue[currentSize];
}
// Add an element to the end (currentSize) of the queue
void enqueue(int *queue, int number) {
    if (currentSize == QUEUE_SIZE - 1) {
        printf("Queue is full\n");
        return;
    }
    int i = 0;
    while (queue[i] != 0) {
        i++;
    }
    queue[i] = number;
    currentSize++;
}

// Remove an element from the front of the queue
int dequeue(int *queue) {
    int i = 0;
    bool isFound = 0;
    int number = queue[i];
    while (queue[i] != 0) {
        queue[i] = queue[i + 1];
        i++;
        isFound = 1;
    }
    if (isFound == 0) {
        printf("Queue is empty\n");
        return -1;
    }
    currentSize--;
    queue[QUEUE_SIZE - currentSize + 2] = 0;
    return number;
}

// Print the queue
void printQueue(int *queue) {
    int i = 0;
    while (queue[i] != 0) {
        printf("%i ", queue[i]);
        i++;
    }
    printf("\n");
}

int main(void) {

    int queue[QUEUE_SIZE] = {0};
    /*  for (int i = 0; i < QUEUE_SIZE; i++){
         queue[i] = 0;
     } */
    // memset(queue, 0, sizeof(int) * QUEUE_SIZE);

    enqueue(queue, 1);
    enqueue(queue, 2);
    printf("Peek: %i\n", peek(queue));
    enqueue(queue, 3);
    enqueue(queue, 4);
    enqueue(queue, 5);
    enqueue(queue, 6);
    dequeue(queue);
    dequeue(queue);

    printQueue(queue);

    return EXIT_SUCCESS;
}