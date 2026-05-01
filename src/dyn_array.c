#include "../include/dyn_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * Dynamic Array
 * 
 * A resizable array that stores void* pointers.
 * 
 * Time Complexities:
 * - Access: O(1)
 * - Push/Pop (end): O(1) amortized
 * - Insert/Remove (middle): O(n)
 * - Search: O(n)
 * - Resize: O(n)
 */

DynArray* dyn_array_create(size_t initial_capacity, 
                           void (*print_fn)(const void *),
                           void (*free_fn)(void *),
                           int (*cmp)(const void *, const void *)) {
    (void)initial_capacity;
    (void)print_fn;
    (void)free_fn;
    (void)cmp;
    // TODO: implement
    return NULL;
}

void dyn_array_destroy(DynArray *arr) {
    (void)arr;
    // TODO: implement
}

int dyn_array_push(DynArray *arr, void *data) {
    (void)arr;
    (void)data;
    // TODO: implement
    return 0;
}

void* dyn_array_pop(DynArray *arr) {
    (void)arr;
    // TODO: implement
    return NULL;
}

int dyn_array_insert(DynArray *arr, size_t index, void *data) {
    (void)arr;
    (void)index;
    (void)data;
    // TODO: implement
    return 0;
}

int dyn_array_remove(DynArray *arr, size_t index) {
    (void)arr;
    (void)index;
    // TODO: implement
    return 0;
}

void* dyn_array_get(DynArray *arr, size_t index) {
    (void)arr;
    (void)index;
    // TODO: implement
    return NULL;
}

int dyn_array_set(DynArray *arr, size_t index, void *data) {
    (void)arr;
    (void)index;
    (void)data;
    // TODO: implement
    return 0;
}

int dyn_array_index_of(DynArray *arr, const void *data) {
    (void)arr;
    (void)data;
    // TODO: implement
    return -1;
}

void dyn_array_clear(DynArray *arr) {
    (void)arr;
    // TODO: implement
}

void dyn_array_print(DynArray *arr) {
    (void)arr;
    // TODO: implement
}

int dyn_array_resize(DynArray *arr, size_t new_capacity) {
    (void)arr;
    (void)new_capacity;
    // TODO: implement
    return 0;
}
