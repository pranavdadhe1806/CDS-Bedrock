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

Heap  *Heap_new(int is_max);
void   Heap_destroy(Heap *heap);

void   _heap_insert_int(Heap *heap, int val);
void   _heap_insert_double(Heap *heap, double val);
void   _heap_insert_char(Heap *heap, char val);
void   _heap_insert_string(Heap *heap, const char *val);

Value *heap_extract(Heap *heap); // caller owns returned Value*
Value *heap_peek(Heap *heap);
void   heap_heapify(Heap *heap, Value **arr, int n);
int    heap_size(Heap *heap);
int    heap_is_empty(Heap *heap);
void   heap_clear(Heap *heap);
void   heap_print(Heap *heap);

#endif /* BEDROCK_HEAP_H */
