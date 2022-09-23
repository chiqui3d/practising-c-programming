#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QUEUE_SIZE 5

struct QueueSocketClient {
    int fd;
    time_t lastAccess;
};
struct Queue {
    struct QueueSocketClient queueSocketClient[QUEUE_SIZE];
    int front;
    int rear;
};

// Check if the queue is empty
bool isEmpty(struct Queue *queue) {
    if (queue->front == -1) {
        return true;
    }
    return false;
}

// Check if the queue is full
bool isFull(struct Queue *queue) {
    if (queue->rear == QUEUE_SIZE - 1) {
        return true;
    }
    return false;
}

// Get the value of the front of the queue without removing it
struct QueueSocketClient peek(struct Queue *queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return (struct QueueSocketClient){0}; // return a struct
    }
    return queue->queueSocketClient[queue->front];
}

void enqueue(struct Queue *queue, struct QueueSocketClient queueSocketClient) {
    if (queue->rear == QUEUE_SIZE - 1) {
        printf("Queue is full\n");
        return;
    }

    if (queue->front == -1) {
        queue->front = 0;
    }
    queue->rear++;

    queue->queueSocketClient[queue->rear] = queueSocketClient;
}

struct QueueSocketClient dequeue(struct Queue *queue) {
    if (queue->front == -1) {
        printf("Queue is empty\n");
        return (struct QueueSocketClient){0}; // return a struct
    }
    struct QueueSocketClient queueSocketClient = queue->queueSocketClient[queue->front];
    queue->front++;

    // 
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1;
    }

    return queueSocketClient;
}

void printQueue(struct Queue queue) {
    if (queue.front == -1) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue:\n");
    printf("Front: %d, Rear: %d\n", queue.front, queue.rear);
    printf("Data:\n");
    int i = queue.front;
    for (; i <= queue.rear; i++) {
        printf("FD: %d, Timestamp: %ld\n", queue.queueSocketClient[i].fd, queue.queueSocketClient[i].lastAccess);
    }
    printf("\n");
}

int main(void) {

    struct Queue queue;
    memset(&queue, 0, sizeof(struct Queue));
    queue.front = -1; // because we start from 0, track the first element of the queue
    queue.rear = -1;  // because we start from 0, track the last element of the queue

    struct QueueSocketClient queueSocketClient = {0};
    queueSocketClient.fd = 1;
    queueSocketClient.lastAccess = time(NULL);
    queue.queueSocketClient[0] = queueSocketClient;

    enqueue(&queue, queueSocketClient);

    queueSocketClient.fd = 2;
    queueSocketClient.lastAccess = time(NULL) + 5;

    enqueue(&queue, queueSocketClient);

    queueSocketClient.fd = 3;
    queueSocketClient.lastAccess = time(NULL) + 10;

    enqueue(&queue, queueSocketClient);

    queueSocketClient.fd = 4;
    queueSocketClient.lastAccess = time(NULL) + 15;

    enqueue(&queue, queueSocketClient);

    queueSocketClient.fd = 5;
    queueSocketClient.lastAccess = time(NULL) + 20;

    enqueue(&queue, queueSocketClient);

    queueSocketClient.fd = 6;
    queueSocketClient.lastAccess = time(NULL) + 25;

    enqueue(&queue, queueSocketClient);

    printf("Peek: %d, Timestamp: %ld\n", peek(&queue).fd, peek(&queue).lastAccess);

    dequeue(&queue);
    dequeue(&queue);


    printQueue(queue);

    return EXIT_SUCCESS;
}