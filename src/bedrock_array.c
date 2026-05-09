#include "../include/bedrock_array.h"
#include <stdio.h>

BRArray *BRArray_new(void) {
    BRArray *arr = malloc(sizeof(BRArray));
    if (arr == NULL) return NULL;
    arr->size = 0;
    arr->capacity = 4;
    arr->data = malloc((size_t)arr->capacity * sizeof(Value *));
    if (arr->data == NULL) {
        free(arr);
        return NULL;
    }
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

static int _ensure_capacity(BRArray *arr) {
    if (arr == NULL) return 0;
    if (arr->size == arr->capacity) {
        int new_capacity = arr->capacity * 2;
        Value **new_data = realloc(arr->data, (size_t)new_capacity * sizeof(Value *));
        if (new_data == NULL) return 0;
        arr->data = new_data;
        arr->capacity = new_capacity;
    }
    return 1;
}

void _brarray_push_int(BRArray *arr, int val) {
    if (!_ensure_capacity(arr)) return;
    Value *v = make_int(val);
    if (v == NULL) return;
    arr->data[arr->size] = v;
    arr->size++;
}

void _brarray_push_double(BRArray *arr, double val) {
    if (!_ensure_capacity(arr)) return;
    Value *v = make_double(val);
    if (v == NULL) return;
    arr->data[arr->size] = v;
    arr->size++;
}

void _brarray_push_char(BRArray *arr, char val) {
    if (!_ensure_capacity(arr)) return;
    Value *v = make_char(val);
    if (v == NULL) return;
    arr->data[arr->size] = v;
    arr->size++;
}

void _brarray_push_string(BRArray *arr, const char *val) {
    if (!_ensure_capacity(arr)) return;
    Value *v = make_string(val);
    if (v == NULL) return;
    arr->data[arr->size] = v;
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
        value_free(val);
        return;
    }

    if (!_ensure_capacity(arr)) {
        value_free(val);
        return;
    }

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
        value_free(val);
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
        arr->data[i] = NULL;
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
