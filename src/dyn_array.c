#include "../include/dyn_array.h"
#include <stdio.h>

DynArray *DynArray_new(void) {
    DynArray *arr = malloc(sizeof(DynArray));
    arr->size = 0;
    arr->capacity = 4;
    arr->data = malloc(arr->capacity * sizeof(Value *));
    return arr;
}

void DynArray_destroy(DynArray *arr) {
    if (arr == NULL) return;
    
    for (int i = 0; i < arr->size; i++) {
        value_free(arr->data[i]);
    }
    free(arr->data);
    free(arr);
}

static void _ensure_capacity(DynArray *arr) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(Value *));
    }
}

void _push_int(DynArray *arr, int val) {
    _ensure_capacity(arr);
    arr->data[arr->size] = make_int(val);
    arr->size++;
}

void _push_double(DynArray *arr, double val) {
    _ensure_capacity(arr);
    arr->data[arr->size] = make_double(val);
    arr->size++;
}

void _push_char(DynArray *arr, char val) {
    _ensure_capacity(arr);
    arr->data[arr->size] = make_char(val);
    arr->size++;
}

void _push_string(DynArray *arr, const char *val) {
    _ensure_capacity(arr);
    arr->data[arr->size] = make_string(val);
    arr->size++;
}

Value *dyn_array_get(DynArray *arr, int index) {
    if (arr == NULL || index < 0 || index >= arr->size) {
        return NULL;
    }
    return arr->data[index];
}

Value *dyn_array_pop(DynArray *arr) {
    if (arr == NULL || arr->size == 0) {
        return NULL;
    }
    Value *val = arr->data[arr->size - 1];
    arr->size--;
    return val;
}

void dyn_array_insert(DynArray *arr, int index, Value *val) {
    if (arr == NULL || val == NULL || index < 0 || index > arr->size) {
        return;
    }
    
    _ensure_capacity(arr);
    
    // Shift elements to the right
    for (int i = arr->size; i > index; i--) {
        arr->data[i] = arr->data[i - 1];
    }
    
    arr->data[index] = val;
    arr->size++;
}

void dyn_array_delete(DynArray *arr, int index) {
    if (arr == NULL || index < 0 || index >= arr->size) {
        return;
    }
    
    value_free(arr->data[index]);
    
    // Shift elements to the left
    for (int i = index; i < arr->size - 1; i++) {
        arr->data[i] = arr->data[i + 1];
    }
    
    arr->size--;
}

void dyn_array_update(DynArray *arr, int index, Value *val) {
    if (arr == NULL || val == NULL || index < 0 || index >= arr->size) {
        return;
    }
    
    value_free(arr->data[index]);
    arr->data[index] = val;
}

int dyn_array_size(DynArray *arr) {
    if (arr == NULL) return 0;
    return arr->size;
}

int dyn_array_contains(DynArray *arr, Value *val) {
    if (arr == NULL || val == NULL) return 0;
    
    for (int i = 0; i < arr->size; i++) {
        if (value_equals(arr->data[i], val)) {
            return 1;
        }
    }
    return 0;
}

void dyn_array_clear(DynArray *arr) {
    if (arr == NULL) return;
    
    for (int i = 0; i < arr->size; i++) {
        value_free(arr->data[i]);
    }
    arr->size = 0;
}

void dyn_array_print(DynArray *arr) {
    if (arr == NULL) {
        printf("[]\n");
        return;
    }
    
    printf("[");
    for (int i = 0; i < arr->size; i++) {
        value_print(arr->data[i]);
        if (i < arr->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}
