#include "../include/heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * Heap (Min-Heap / Max-Heap)
 * 
 * A binary heap that can be configured as either min-heap or max-heap.
 * 
 * Time Complexities:
 * - Insert: O(log n)
 * - Extract: O(log n)
 * - Peek: O(1)
 * - Heapify: O(n)
 */

Heap* heap_create(size_t initial_capacity, HeapType type,
                  int (*cmp)(const void *, const void *),
                  void (*print_fn)(const void *),
                  void (*free_fn)(void *)) {
    (void)initial_capacity;
    (void)type;
    (void)cmp;
    (void)print_fn;
    (void)free_fn;
    // TODO: implement
    return NULL;
}

void heap_destroy(Heap *heap) {
    (void)heap;
    // TODO: implement
}

int heap_insert(Heap *heap, void *data) {
    (void)heap;
    (void)data;
    // TODO: implement
    return 0;
}

void* heap_extract(Heap *heap) {
    (void)heap;
    // TODO: implement
    return NULL;
}

void* heap_peek(Heap *heap) {
    (void)heap;
    // TODO: implement
    return NULL;
}

size_t heap_size(Heap *heap) {
    (void)heap;
    // TODO: implement
    return 0;
}

int heap_is_empty(Heap *heap) {
    (void)heap;
    // TODO: implement
    return 0;
}

void heap_clear(Heap *heap) {
    (void)heap;
    // TODO: implement
}

void heap_print(Heap *heap) {
    (void)heap;
    // TODO: implement
}

int heap_heapify(Heap *heap, void **array, size_t n) {
    (void)heap;
    (void)array;
    (void)n;
    // TODO: implement
    return 0;
}
