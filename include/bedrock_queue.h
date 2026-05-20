#ifndef BEDROCK_QUEUE_H
#define BEDROCK_QUEUE_H

#include "value.h"

typedef struct {
    Value **data;       // Circular buffer array of Value pointers
    int     head;       // Index of front element
    int     tail;       // Index of next empty slot
    int     size;       // Current number of elements
    int     capacity;   // Current allocated capacity
} BRQueue;

/* Lifecycle */
BRQueue* BRQueue_new(void);                              /* caller owns */
void     BRQueue_destroy(BRQueue *queue);                /* frees queue + all contained Values */

/* Type-specific enqueue — queue takes ownership of created Value */
void _brqueue_enqueue_int(BRQueue *queue, int val);
void _brqueue_enqueue_double(BRQueue *queue, double val);
void _brqueue_enqueue_char(BRQueue *queue, char val);
void _brqueue_enqueue_string(BRQueue *queue, const char *val);

/* Returns OWNED pointer; caller MUST call value_free() */
Value* brqueue_dequeue(BRQueue *queue);

/* Returns BORROWED pointer; caller must NOT free */
Value* brqueue_peek(BRQueue *queue);

int    brqueue_is_empty(BRQueue *queue);
int    brqueue_size(BRQueue *queue);
void   brqueue_clear(BRQueue *queue);                    /* frees all contained Values */
void   brqueue_print(BRQueue *queue);

#endif // BEDROCK_QUEUE_H
