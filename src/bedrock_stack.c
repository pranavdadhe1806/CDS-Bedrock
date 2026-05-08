#include "../include/bedrock_stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_CAPACITY 4

// Internal helper to ensure capacity (double when full)
static void _ensure_capacity(BRStack *stack) {
    if (stack->top >= stack->capacity) {
        int new_capacity = stack->capacity * 2;
        Value **new_data = realloc(stack->data, new_capacity * sizeof(Value*));
        if (new_data == NULL) return;  // Allocation failed
        stack->data = new_data;
        stack->capacity = new_capacity;
    }
}

BRStack* BRStack_new(void) {
    BRStack *stack = malloc(sizeof(BRStack));
    if (stack == NULL) return NULL;
    
    stack->data = malloc(INITIAL_CAPACITY * sizeof(Value*));
    if (stack->data == NULL) {
        free(stack);
        return NULL;
    }
    
    stack->top = 0;
    stack->capacity = INITIAL_CAPACITY;
    return stack;
}

void BRStack_destroy(BRStack *stack) {
    if (stack == NULL) return;
    brstack_clear(stack);
    free(stack->data);
    free(stack);
}

// Type-specific push implementations
void _brstack_push_int(BRStack *stack, int val) {
    _ensure_capacity(stack);
    stack->data[stack->top] = make_int(val);
    stack->top++;
}

void _brstack_push_double(BRStack *stack, double val) {
    _ensure_capacity(stack);
    stack->data[stack->top] = make_double(val);
    stack->top++;
}

void _brstack_push_char(BRStack *stack, char val) {
    _ensure_capacity(stack);
    stack->data[stack->top] = make_char(val);
    stack->top++;
}

void _brstack_push_string(BRStack *stack, const char *val) {
    _ensure_capacity(stack);
    stack->data[stack->top] = make_string(val);
    stack->top++;
}

void brstack_pop(BRStack *stack) { // Free internally to prevent memory leaks
    if (stack == NULL || stack->top == 0) {
        return;
    }
    stack->top--;
    value_free(stack->data[stack->top]); // Free the Value before shrinking
}

Value* brstack_peek(BRStack *stack) {
    if (stack == NULL || stack->top == 0) return NULL;
    return stack->data[stack->top - 1];
}

int brstack_is_empty(BRStack *stack) {
    if (stack == NULL) return 1;
    return stack->top == 0;
}

int brstack_size(BRStack *stack) {
    if (stack == NULL) return 0;
    return stack->top;
}

void brstack_clear(BRStack *stack) {
    if (stack == NULL) return;
    
    for (int i = 0; i < stack->top; i++) {
        value_free(stack->data[i]);
    }
    stack->top = 0;
}

void brstack_print(BRStack *stack) {
    if (stack == NULL || stack->top == 0) {
        printf("Bottom: [] :Top\n");
        return;
    }
    
    printf("Bottom: [");
    for (int i = 0; i < stack->top; i++) {
        value_print(stack->data[i]);
        if (i < stack->top - 1) {
            printf(", ");
        }
    }
    printf("] :Top\n");
}
