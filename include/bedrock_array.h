#ifndef BEDROCK_ARRAY_H
#define BEDROCK_ARRAY_H

#include "value.h"

typedef struct {
    Value **data;       // Array of Value pointers
    int     size;       // Current number of elements
    int     capacity;   // Current allocated capacity
} BRArray;

BRArray *BRArray_new(void);
void      BRArray_destroy(BRArray *arr);

void      _brarray_push_int(BRArray *arr, int val);
void      _brarray_push_double(BRArray *arr, double val);
void      _brarray_push_char(BRArray *arr, char val);
void      _brarray_push_string(BRArray *arr, const char *val);

Value    *brarray_get(BRArray *arr, int index);
void      brarray_pop(BRArray *arr); // Changed to void to prevent memory leaks
void      brarray_insert(BRArray *arr, int index, Value *val);
void      brarray_delete(BRArray *arr, int index);
void      brarray_update(BRArray *arr, int index, Value *val);
int       brarray_size(BRArray *arr);
int       brarray_contains(BRArray *arr, Value *val);
void      brarray_clear(BRArray *arr);
void      brarray_print(BRArray *arr);

#endif // BEDROCK_ARRAY_H
