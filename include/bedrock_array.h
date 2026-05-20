#ifndef BEDROCK_ARRAY_H
#define BEDROCK_ARRAY_H

#include "value.h"

typedef struct {
    Value **data;       // Array of Value pointers
    int     size;       // Current number of elements
    int     capacity;   // Current allocated capacity
} BRArray;

/* Lifecycle */
BRArray *BRArray_new(void);                             /* caller owns */
void      BRArray_destroy(BRArray *arr);                /* frees array + all contained Values */

/* Type-specific push — array takes ownership of created Value */
void      _brarray_push_int(BRArray *arr, int val);
void      _brarray_push_double(BRArray *arr, double val);
void      _brarray_push_char(BRArray *arr, char val);
void      _brarray_push_string(BRArray *arr, const char *val);

/* Access — returns BORROWED pointer; caller must NOT free */
Value    *brarray_get(const BRArray *arr, int index);

/* Removal — returns OWNED pointer; caller MUST call value_free() */
Value    *brarray_pop(BRArray *arr);

/* Mutation — array takes ownership of val on success, frees old value */
void      brarray_insert(BRArray *arr, int index, Value *val);
void      brarray_delete(BRArray *arr, int index);       /* frees removed Value internally */
void      brarray_update(BRArray *arr, int index, Value *val);

int       brarray_size(const BRArray *arr);
int       brarray_contains(const BRArray *arr, Value *val);    /* borrows val for comparison */
void      brarray_clear(BRArray *arr);                   /* frees all contained Values */
void      brarray_print(const BRArray *arr);

#endif // BEDROCK_ARRAY_H
