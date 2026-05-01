#include "../include/queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * Queue (Circular Buffer)
 * 
 * A FIFO (First In, First Out) data structure implemented with a circular buffer.
 * 
 * Time Complexities:
 * - Enqueue: O(1) amortized
 * - Dequeue: O(1)
 * - Peek: O(1)
 * - Is Empty: O(1)
 */

Queue* queue_create(size_t initial_capacity,
                    void (*print_fn)(const void *),
                    void (*free_fn)(void *)) {
    (void)initial_capacity;
    (void)print_fn;
    (void)free_fn;
    // TODO: implement
    return NULL;
}

void queue_destroy(Queue *queue) {
    (void)queue;
    // TODO: implement
}

int queue_enqueue(Queue *queue, void *data) {
    (void)queue;
    (void)data;
    // TODO: implement
    return 0;
}

void* queue_dequeue(Queue *queue) {
    (void)queue;
    // TODO: implement
    return NULL;
}

void* queue_peek(Queue *queue) {
    (void)queue;
    // TODO: implement
    return NULL;
}

int queue_is_empty(Queue *queue) {
    (void)queue;
    // TODO: implement
    return 0;
}

int queue_is_full(Queue *queue) {
    (void)queue;
    // TODO: implement
    return 0;
}

size_t queue_size(Queue *queue) {
    (void)queue;
    // TODO: implement
    return 0;
}

void queue_clear(Queue *queue) {
    (void)queue;
    // TODO: implement
}

void queue_print(Queue *queue) {
    (void)queue;
    // TODO: implement
}
