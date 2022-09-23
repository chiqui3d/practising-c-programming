#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// binary heap(min-heap) implementation in C using array of struct data structure

#define MAX_HEAP_SIZE 10
#define DATETIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define DATETIME_SIZE 20

struct ElementHeap {
    time_t priorityTime;
    int extraData; // nothing in this example
};
struct Heap {
    struct ElementHeap elements[MAX_HEAP_SIZE];
    int currentSize;
    int capacity;
};

struct Heap createHeap();
void insertNodeHeap(struct Heap *heap, time_t priorityTime);
void deleteMinHeap(struct Heap *heap);
struct Heap heapSort(struct Heap *heap);
void heapify(struct Heap *heap, int index);

void swapElementHeap(struct ElementHeap *a, struct ElementHeap *b);
int leftChildHeap(int element);
int rightChildHeap(int element);
int parentHeap(int element);
void changePriorityTime(struct Heap *heap, int index, time_t priorityTime);

void printHeap(struct Heap heap);
void printHeapSort(struct Heap *heap);


int main(void) {

    struct Heap heap = createHeap();
    srand(time(NULL)); // Initialization, should only be called once.
    int nrand;

    for (int i = 0; i < 10; i++) {
        nrand = rand() % (3600); // Returns a pseudo-random integer between 0 and 3600 (1 hour)
        insertNodeHeap(&heap, time(NULL) + (i*nrand));
    }

    changePriorityTime(&heap, 5, time(NULL) - 3600); // 24 hours

    deleteMinHeap(&heap); // delete min

    printHeap(heap);
    printf("Heap sort:\n");
    printHeapSort(&heap);

    return 0;
}

struct Heap createHeap() {
    struct Heap heap;
    memset(heap.elements, 0, sizeof(heap.elements));
    heap.currentSize = 0;
    heap.capacity = (int)MAX_HEAP_SIZE;
    return heap;
}

void insertNodeHeap(struct Heap *heap, time_t priorityTime) {
    if (heap->currentSize == heap->capacity) {
        printf("Heap is full. The element %ld cannot be inserted\n", priorityTime);
        return;
    }

     // insert in the last position
    heap->elements[heap->currentSize] = (struct ElementHeap) {priorityTime, 0};
    heap->currentSize++;
    int i = heap->currentSize - 1; // last element
    // loop until the element is minor than its parent or it is the root
    // shiftUp
    while (i != 0 && difftime(priorityTime, heap->elements[parentHeap(i)].priorityTime) < 0) {
        swapElementHeap(&heap->elements[i], &heap->elements[parentHeap(i)]);
        i = parentHeap(i);
    }
}

void changePriorityTime(struct Heap *heap, int index, time_t priorityTime) {
    if (index < 0 || index >= heap->currentSize) {
        printf("Index %d is out of range\n", index);
        return;
    }
    time_t oldPriorityTime = heap->elements[index].priorityTime;
    heap->elements[index].priorityTime = priorityTime;

    if (difftime(priorityTime, oldPriorityTime) < 0) { 
        printf("Shift up\n");
        // shiftUp
        while (index != 0 && difftime(priorityTime, heap->elements[parentHeap(index)].priorityTime) < 0) {
            swapElementHeap(&heap->elements[index], &heap->elements[parentHeap(index)]);
            index = parentHeap(index);
        }
    } else {
        printf("Shift down\n");
        // shiftDown
        heapify(heap, index);
    }
}

void deleteMinHeap(struct Heap *heap) {
    if (heap->currentSize == 0) {
        printf("Heap is empty\n");
        return;
    }
    // replace the element with the last element
    heap->elements[0] = heap->elements[heap->currentSize - 1];
    heap->elements[heap->currentSize - 1] = (struct ElementHeap) {0, 0};
    heap->currentSize--;
    // heapify
    heapify(heap, 0);
}

struct Heap heapSort(struct Heap *heap) {
    struct Heap sortedHeap = createHeap();
    int i = 0;
    while (heap->currentSize != 0) {
        sortedHeap.elements[i] = heap->elements[0];
        heap->elements[0] = heap->elements[heap->currentSize - 1];
        heap->currentSize--;
        heapify(heap, 0);
        i++;
    }
    sortedHeap.currentSize = i;
    return sortedHeap;
}

// shiftDown
void heapify(struct Heap *heap, int index) {
    int left = leftChildHeap(index);
    int right = rightChildHeap(index);
    int smallest = index;

    if (left < heap->currentSize && difftime(heap->elements[left].priorityTime, heap->elements[smallest].priorityTime) < 0) {
        smallest = left;
    }
    if (right < heap->currentSize && difftime(heap->elements[right].priorityTime, heap->elements[smallest].priorityTime) < 0) {
        smallest = right;
    }
    if (smallest != index) { // if the smallest is not the current index, then swap
        swapElementHeap(&heap->elements[index], &heap->elements[smallest]);
        heapify(heap, smallest);
    }
}

char *formatTimeToString(time_t time, char *format) {
    struct tm *timeInfo = localtime(&time);
    strftime(format, DATETIME_SIZE, DATETIME_FORMAT, timeInfo);
    return format;
}


int leftChildHeap(int element) {
    return 2 * element + 1;
}

int rightChildHeap(int element) {
    return 2 * element + 2;
}

int parentHeap(int element) {
    return (element - 1) / 2;
}

void swapElementHeap(struct ElementHeap *a, struct ElementHeap *b) {
    struct ElementHeap temp = *a;
    *a = *b;
    *b = temp;
}

// print in level order
void printHeap(struct Heap heap) {
    printf("Size: %d, Capacity: %d\n", heap.currentSize, heap.capacity);
    int i;
    for (i = 0; i < heap.currentSize; i++) {
        char timeString[DATETIME_SIZE];
        printf("Index: %d, Time: %ld, Format: %s\n", i, heap.elements[i].priorityTime, formatTimeToString(heap.elements[i].priorityTime, timeString));
    }
    printf("\n");
}

// print in time order
void printHeapSort(struct Heap *heap) {
    struct Heap sortedHeap = heapSort(heap);
    for (int i = 0; i < sortedHeap.currentSize; i++) {
        char timeString[DATETIME_SIZE];
        printf("Index: %d, Time: %ld, Format: %s\n", i, sortedHeap.elements[i].priorityTime, formatTimeToString(sortedHeap.elements[i].priorityTime, timeString));
    }
    printf("\n");
}