#include "../include/bedrock_array.h"
#include <stdio.h>

BRArray *BRArray_new(void) {
    BRArray *arr = malloc(sizeof(BRArray));
    arr->size = 0;
    arr->capacity = 4;
    arr->data = malloc(arr->capacity * sizeof(Value *));
    return arr;
}

void BRArray_destroy(BRArray *arr) {
    if (arr == NULL) return;

    for (int i = 0; i < arr->size; i++) {
        value_free(arr->data[i]);
    }
    free(arr->data);
    free(arr);
}

static void _ensure_capacity(BRArray *arr) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(Value *));
    }
}

void _brarray_push_int(BRArray *arr, int val) {
    _ensure_capacity(arr);
    arr->data[arr->size] = make_int(val);
    arr->size++;
}

void _brarray_push_double(BRArray *arr, double val) {
    _ensure_capacity(arr);
    arr->data[arr->size] = make_double(val);
    arr->size++;
}

void _brarray_push_char(BRArray *arr, char val) {
    _ensure_capacity(arr);
    arr->data[arr->size] = make_char(val);
    arr->size++;
}

void _brarray_push_string(BRArray *arr, const char *val) {
    _ensure_capacity(arr);
    arr->data[arr->size] = make_string(val);
    arr->size++;
}

Value *brarray_get(BRArray *arr, int index) {
    if (arr == NULL || index < 0 || index >= arr->size) {
        return NULL;
    }
    return arr->data[index];
}

Value *brarray_pop(BRArray *arr) {
    if (arr == NULL || arr->size == 0) {
        return NULL;
    }
    Value *value = arr->data[arr->size - 1];
    arr->data[arr->size - 1] = NULL;
    arr->size--;
    return value;
}

void brarray_insert(BRArray *arr, int index, Value *val) {
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

void brarray_delete(BRArray *arr, int index) {
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

void brarray_update(BRArray *arr, int index, Value *val) {
    if (arr == NULL || val == NULL || index < 0 || index >= arr->size) {
        return;
    }

    value_free(arr->data[index]);
    arr->data[index] = val;
}

int brarray_size(BRArray *arr) {
    if (arr == NULL) return 0;
    return arr->size;
}

int brarray_contains(BRArray *arr, Value *val) {
    if (arr == NULL || val == NULL) return 0;

    for (int i = 0; i < arr->size; i++) {
        if (value_equals(arr->data[i], val)) {
            return 1;
        }
    }
    return 0;
}

void brarray_clear(BRArray *arr) {
    if (arr == NULL) return;

    for (int i = 0; i < arr->size; i++) {
        value_free(arr->data[i]);
    }
    arr->size = 0;
}

void brarray_print(BRArray *arr) {
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
