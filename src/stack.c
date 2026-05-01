#include "../include/stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * Stack
 * 
 * A LIFO (Last In, First Out) data structure implemented with a dynamic array.
 * 
 * Time Complexities:
 * - Push: O(1) amortized
 * - Pop: O(1)
 * - Peek: O(1)
 * - Is Empty: O(1)
 */

Stack* stack_create(size_t initial_capacity,
                    void (*print_fn)(const void *),
                    void (*free_fn)(void *)) {
    (void)initial_capacity;
    (void)print_fn;
    (void)free_fn;
    // TODO: implement
    return NULL;
}

void stack_destroy(Stack *stack) {
    (void)stack;
    // TODO: implement
}

int stack_push(Stack *stack, void *data) {
    (void)stack;
    (void)data;
    // TODO: implement
    return 0;
}

void* stack_pop(Stack *stack) {
    (void)stack;
    // TODO: implement
    return NULL;
}

void* stack_peek(Stack *stack) {
    (void)stack;
    // TODO: implement
    return NULL;
}

int stack_is_empty(Stack *stack) {
    (void)stack;
    // TODO: implement
    return 0;
}

size_t stack_size(Stack *stack) {
    (void)stack;
    // TODO: implement
    return 0;
}

void stack_clear(Stack *stack) {
    (void)stack;
    // TODO: implement
}

void stack_print(Stack *stack) {
    (void)stack;
    // TODO: implement
}
