#ifndef BEDROCK_STACK_H
#define BEDROCK_STACK_H

#include "value.h"

typedef struct {
    Value **data;       // Array of Value pointers
    int     top;        // Index of next empty slot (also == size)
    int     capacity;   // Current allocated capacity
} BRStack;

// Lifecycle
BRStack* BRStack_new(void);
void     BRStack_destroy(BRStack *stack);

// Type-specific push functions (called by _Generic macro)
void _brstack_push_int(BRStack *stack, int val);
void _brstack_push_double(BRStack *stack, double val);
void _brstack_push_char(BRStack *stack, char val);
void _brstack_push_string(BRStack *stack, const char *val);

// Core operations
void   brstack_pop(BRStack *stack); // Changed to void to prevent memory leaks
Value* brstack_peek(BRStack *stack);
int    brstack_is_empty(BRStack *stack);
int    brstack_size(BRStack *stack);
void   brstack_clear(BRStack *stack);
void   brstack_print(BRStack *stack);

#endif // BEDROCK_STACK_H
