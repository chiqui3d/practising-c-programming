#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// binary heap(min-heap) implementation in C using array data structure

#define MAX_HEAP_SIZE 10

struct Heap {
    int elements[MAX_HEAP_SIZE];
    int currentSize;
    int capacity;
};

struct Heap createHeap();
int leftChildHeap(int element);
int rightChildHeap(int element);
int parentHeap(int element);
void swapNodeHeap(int *a, int *b);
void insertNodeHeap(struct Heap *heap, int element);
void deleteMinHeap(struct Heap *heap);
struct Heap heapSort(struct Heap heap);
void heapify(struct Heap *heap, int index);
int binarySearchHeap(struct Heap *heap, int search);
void printHeap(struct Heap heap);
void printHeapInOrder(struct Heap heap, int index);
void printHeapSort(struct Heap heap);

// https://es.wikipedia.org/wiki/Mont%C3%ADculo_binario
// https://en.wikipedia.org/wiki/Heap_%28data_structure%29#Comparison_of_theoretic_bounds_for_variants
// min heap
int main(void) {

    struct Heap heap = createHeap();

    // insert
    insertNodeHeap(&heap, 10);
    insertNodeHeap(&heap, 20);
    insertNodeHeap(&heap, 30);
    insertNodeHeap(&heap, 5);
    insertNodeHeap(&heap, 15);
    insertNodeHeap(&heap, 25);
    insertNodeHeap(&heap, 35);
    insertNodeHeap(&heap, 40);
    insertNodeHeap(&heap, 45);
    insertNodeHeap(&heap, 100);
    insertNodeHeap(&heap, 50);

    binarySearchHeap(&heap, 100);

    //
    printHeap(heap);
    printHeapSort(heap);
    printf("\n");
    deleteMinHeap(&heap);
    deleteMinHeap(&heap);
    printHeap(heap);
    printHeapSort(heap);
    printf("\n");

    return 0;
}

struct Heap createHeap() {
    struct Heap heap;
    memset(heap.elements, 0, sizeof(heap.elements));
    heap.currentSize = 0;
    heap.capacity = (int)MAX_HEAP_SIZE;
    return heap;
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

// swap the children with the parent
void swapNodeHeap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


void insertNodeHeap(struct Heap *heap, int element) {
    if (heap->currentSize == heap->capacity) {
        printf("Heap is full. The element %d cannot be inserted\n", element);
        return;
    }
    // insert in the last position
    heap->elements[heap->currentSize] = element;
    heap->currentSize++;
    int i = heap->currentSize - 1; // last element
    // loop until the element is minor than its parent or it is the root
    while (i != 0 && element <= heap->elements[parentHeap(i)]) {
        swapNodeHeap(&heap->elements[i], &heap->elements[parentHeap(i)]);
        i = parentHeap(i);
    }
}

void deleteMinHeap(struct Heap *heap) {
    if (heap->currentSize == 0) {
        printf("Heap is empty\n");
        return;
    }
    // replace the element with the last element
    heap->elements[0] = heap->elements[heap->currentSize - 1];
    heap->elements[heap->currentSize - 1] = 0;
    heap->currentSize--;
    // heapify
    heapify(heap, 0);
}


int binarySearchHeap(struct Heap *heap, int search) {
    int low = 0;
    int high = heap->capacity -1;
    int mid;
    if (low > high) {
        printf("Element not found\n");
        return -1;
    }

    while (low <= high) {
        mid = (low + high) / 2;
        printf("mid: %i\n", mid);
        if (heap->elements[mid] == search) {
            printf("Element %d found at index %d\n",search, mid);
            return mid;
        } else if (search > heap->elements[mid]) { // number is on the right side
            low = mid + 1;
        } else { // number is on the left side
            high = mid - 1;
        }
    }

    printf("Element not found\n");

    return -1;
}

struct Heap heapSort(struct Heap heap) {
    struct Heap sortedHeap = createHeap();
    int i = 0;
    while (heap.currentSize != 0) {
        sortedHeap.elements[i] = heap.elements[0];
        heap.elements[0] = heap.elements[heap.currentSize - 1];
        heap.currentSize--;
        heapify(&heap, 0);
        i++;
    }
    sortedHeap.currentSize = i;
    return sortedHeap;
}


void heapify(struct Heap *heap, int index) {
    int left = leftChildHeap(index);
    int right = rightChildHeap(index);
    int smallest = index;
    if (left < heap->currentSize && heap->elements[left] < heap->elements[smallest]) {
        smallest = left;
    }
    if (right < heap->currentSize && heap->elements[right] < heap->elements[smallest]) {
        smallest = right;
    }
    if (smallest != index) { // if the smallest is not the current index
        swapNodeHeap(&heap->elements[index], &heap->elements[smallest]);
        heapify(heap, smallest);
    }
}

void printHeapSort(struct Heap heap) {
    struct Heap sortedHeap = heapSort(heap);
    for (int i = 0; i < sortedHeap.currentSize; i++) {
        printf("%d ", sortedHeap.elements[i]);
    }
    printf("\n");
}

// print in level order
void printHeap(struct Heap heap) {
    printf("Size: %d, Capacity: %d\n", heap.currentSize, heap.capacity);
    int i;
    for (i = 0; i < heap.currentSize; i++) {
        printf("%d ", heap.elements[i]);
    }
    printf("\n");
}

void printHeapInOrder(struct Heap heap, int index) {
    if (index >= heap.currentSize) {
        return;
    }
    printHeapInOrder(heap, leftChildHeap(index));
    printf("%d ", heap.elements[index]);
    printHeapInOrder(heap, rightChildHeap(index));
}
