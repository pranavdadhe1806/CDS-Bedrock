#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>

typedef enum { MIN_HEAP, MAX_HEAP } HeapType;

typedef struct {
    void     **data;
    size_t     size;
    size_t     capacity;
    HeapType   type;
    int      (*cmp)(const void *, const void *);
    void     (*print_fn)(const void *);
    void     (*free_fn)(void *);
} Heap;

// Create a new heap with initial capacity and type
Heap* heap_create(size_t initial_capacity, HeapType type,
                  int (*cmp)(const void *, const void *),
                  void (*print_fn)(const void *),
                  void (*free_fn)(void *));

// Destroy the heap and free all memory
void heap_destroy(Heap *heap);

// Insert an element into the heap
int heap_insert(Heap *heap, void *data);

// Remove and return the root element
void* heap_extract(Heap *heap);

// Peek at the root element without removing it
void* heap_peek(Heap *heap);

// Get the number of elements in the heap
size_t heap_size(Heap *heap);

// Check if the heap is empty
int heap_is_empty(Heap *heap);

// Remove all elements (calls free_fn if provided)
void heap_clear(Heap *heap);

// Print all elements using print_fn
void heap_print(Heap *heap);

// Build a heap from an existing array
int heap_heapify(Heap *heap, void **array, size_t n);

#endif // HEAP_H
