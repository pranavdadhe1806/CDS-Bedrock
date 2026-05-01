#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void   **data;        // circular buffer
    size_t   head;        // index of front element
    size_t   tail;        // index of next empty slot
    size_t   size;
    size_t   capacity;
    void   (*print_fn)(const void *);
    void   (*free_fn)(void *);
} Queue;

// Create a new queue with initial capacity
Queue* queue_create(size_t initial_capacity,
                    void (*print_fn)(const void *),
                    void (*free_fn)(void *));

// Destroy the queue and free all memory
void queue_destroy(Queue *queue);

// Add an element to the back of the queue
int queue_enqueue(Queue *queue, void *data);

// Remove and return the front element
void* queue_dequeue(Queue *queue);

// Peek at the front element without removing it
void* queue_peek(Queue *queue);

// Check if the queue is empty
int queue_is_empty(Queue *queue);

// Check if the queue is full
int queue_is_full(Queue *queue);

// Get the number of elements in the queue
size_t queue_size(Queue *queue);

// Remove all elements (calls free_fn if provided)
void queue_clear(Queue *queue);

// Print all elements using print_fn
void queue_print(Queue *queue);

#endif // QUEUE_H
