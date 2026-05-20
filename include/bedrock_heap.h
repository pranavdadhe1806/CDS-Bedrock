#ifndef BEDROCK_HEAP_H
#define BEDROCK_HEAP_H

#include "value.h"

typedef struct {
    Value **data;
    int     size;
    int     capacity;
    int     is_max;
} Heap;

static inline int _parent(int i)      { return (i - 1) / 2; }
static inline int _left_child(int i)  { return 2 * i + 1;   }
static inline int _right_child(int i) { return 2 * i + 2;   }

/* Lifecycle */
Heap  *Heap_new(int is_max);                            /* caller owns; is_max=1 for max-heap, 0 for min-heap */
void   Heap_destroy(Heap *heap);                        /* frees heap + all contained Values */

/* Type-specific insert — heap takes ownership of created Value */
void   _heap_insert_int(Heap *heap, int val);
void   _heap_insert_double(Heap *heap, double val);
void   _heap_insert_char(Heap *heap, char val);
void   _heap_insert_string(Heap *heap, const char *val);

/* Returns OWNED pointer; caller MUST call value_free(). Returns NULL if empty. */
Value *heap_extract(Heap *heap);

/* Returns BORROWED pointer; caller must NOT free. Returns NULL if empty. */
Value *heap_peek(const Heap *heap);

/* Heap takes ownership of all arr[0..n-1] elements */
void   heap_heapify(Heap *heap, Value **arr, int n);

int    heap_size(const Heap *heap);
int    heap_is_empty(const Heap *heap);
void   heap_clear(Heap *heap);                          /* frees all contained Values */
void   heap_print(const Heap *heap);

#endif /* BEDROCK_HEAP_H */
