#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void   **data;        // internal array
    size_t   top;         // index of next empty slot
    size_t   capacity;
    void   (*print_fn)(const void *);
    void   (*free_fn)(void *);
} Stack;

// Create a new stack with initial capacity
Stack* stack_create(size_t initial_capacity,
                    void (*print_fn)(const void *),
                    void (*free_fn)(void *));

// Destroy the stack and free all memory
void stack_destroy(Stack *stack);

// Push an element onto the stack
int stack_push(Stack *stack, void *data);

// Pop and return the top element
void* stack_pop(Stack *stack);

// Peek at the top element without removing it
void* stack_peek(Stack *stack);

// Check if the stack is empty
int stack_is_empty(Stack *stack);

// Get the number of elements in the stack
size_t stack_size(Stack *stack);

// Remove all elements (calls free_fn if provided)
void stack_clear(Stack *stack);

// Print all elements using print_fn
void stack_print(Stack *stack);

#endif // STACK_H
