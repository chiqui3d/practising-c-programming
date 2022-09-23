#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_SIZE 5
int frontQueue = -1; // because we start from 0, track the first element of the queue
int rearQueue = -1;  // because we start from 0, track the last element of the queue

// Check if the queue is empty
bool isEmpty() {
    if (frontQueue == -1) {
        return true;
    }
    return false;
}

// Check if the queue is full
bool isFull() {
    if (rearQueue == QUEUE_SIZE - 1) {
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
    return queue[frontQueue];
}

void enqueue(int *queue, int number) {
    if (rearQueue == QUEUE_SIZE - 1) {
        printf("Queue is full\n");
        return;
    }

    if (frontQueue == -1) {
        frontQueue = 0;
    }
    rearQueue++;

    queue[rearQueue] = number;
    printf("\nInserted: %d\n", number);
}

int dequeue(int *queue) {
    if (frontQueue == -1) {
        printf("Queue is empty\n");
        return -1;
    }
    int number = queue[frontQueue];
    printf("\nDeleted: %d\n", number);
    frontQueue++;

    if (frontQueue > rearQueue) {
        frontQueue = rearQueue = -1;
    }
    return number;
}

void printQueue(int *queue) {
    if (frontQueue == -1) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue: ");
    for (int i = frontQueue; i <= rearQueue; i++) {
        printf("%i ", queue[i]);
    }
    printf("\n");
}

int main(void) {

    int queue[QUEUE_SIZE];

    enqueue(queue, 1);
    enqueue(queue, 2);
    enqueue(queue, 3);
    enqueue(queue, 4);
    enqueue(queue, 5);
    dequeue(queue);
    // we can only add indexes 0 and 1 only when the queue is reset (when all the elements have been dequeued).
    enqueue(queue, 1); // fails because the queue is full

    printQueue(queue);

    return 0;
}