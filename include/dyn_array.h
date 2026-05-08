#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include "value.h"

typedef struct {
    Value **data;      // buffer of Value pointers
    int     size;      // current number of elements
    int     capacity;  // allocated slots
} DynArray;

DynArray *DynArray_new(void);
void      DynArray_destroy(DynArray *arr);

void      _push_int(DynArray *arr, int val);
void      _push_double(DynArray *arr, double val);
void      _push_char(DynArray *arr, char val);
void      _push_string(DynArray *arr, const char *val);

Value    *dyn_array_get(DynArray *arr, int index);
Value    *dyn_array_pop(DynArray *arr);
void      dyn_array_insert(DynArray *arr, int index, Value *val);
void      dyn_array_delete(DynArray *arr, int index);
void      dyn_array_update(DynArray *arr, int index, Value *val);
int       dyn_array_size(DynArray *arr);
int       dyn_array_contains(DynArray *arr, Value *val);
void      dyn_array_clear(DynArray *arr);
void      dyn_array_print(DynArray *arr);

#endif // DYN_ARRAY_H
