#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void   **data;        // array of void* pointers
    size_t   size;        // number of elements currently stored
    size_t   capacity;    // allocated slots
    void   (*print_fn)(const void *);
    void   (*free_fn)(void *);
    int    (*cmp)(const void *, const void *);
} DynArray;

// Create a new dynamic array with initial capacity
DynArray* dyn_array_create(size_t initial_capacity, 
                           void (*print_fn)(const void *),
                           void (*free_fn)(void *),
                           int (*cmp)(const void *, const void *));

// Destroy the array and free all memory
void dyn_array_destroy(DynArray *arr);

// Add an element to the end of the array
int dyn_array_push(DynArray *arr, void *data);

// Remove and return the last element
void* dyn_array_pop(DynArray *arr);

// Insert an element at a specific index
int dyn_array_insert(DynArray *arr, size_t index, void *data);

// Remove an element at a specific index
int dyn_array_remove(DynArray *arr, size_t index);

// Get element at index without removing it
void* dyn_array_get(DynArray *arr, size_t index);

// Set element at index
int dyn_array_set(DynArray *arr, size_t index, void *data);

// Search for an element using the comparator
int dyn_array_index_of(DynArray *arr, const void *data);

// Remove all elements (calls free_fn if provided)
void dyn_array_clear(DynArray *arr);

// Print all elements using print_fn
void dyn_array_print(DynArray *arr);

// Resize the array to new capacity
int dyn_array_resize(DynArray *arr, size_t new_capacity);

#endif // DYN_ARRAY_H
