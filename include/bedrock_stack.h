#ifndef BEDROCK_STACK_H
#define BEDROCK_STACK_H

#include "value.h"

typedef struct {
    Value **data;       // Array of Value pointers
    int     top;        // Index of next empty slot (also == size)
    int     capacity;   // Current allocated capacity
} BRStack;

/* Lifecycle */
BRStack* BRStack_new(void);                             /* caller owns */
void     BRStack_destroy(BRStack *stack);               /* frees stack + all contained Values */

/* Type-specific push — stack takes ownership of created Value */
void _brstack_push_int(BRStack *stack, int val);
void _brstack_push_double(BRStack *stack, double val);
void _brstack_push_char(BRStack *stack, char val);
void _brstack_push_string(BRStack *stack, const char *val);

/* Returns OWNED pointer; caller MUST call value_free() */
Value* brstack_pop(BRStack *stack);

/* Returns BORROWED pointer; caller must NOT free */
Value* brstack_peek(const BRStack *stack);

int    brstack_is_empty(const BRStack *stack);
int    brstack_size(const BRStack *stack);
void   brstack_clear(BRStack *stack);                   /* frees all contained Values */
void   brstack_print(const BRStack *stack);

#endif // BEDROCK_STACK_H
