#include "../include/bedrock_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static int _heap_cmp(Heap *heap, int i, int j) {
    int cmp = value_compare(heap->data[i], heap->data[j]);
    return heap->is_max ? cmp : -cmp;
}

static int _ensure_capacity(Heap *heap) {
    if (heap == NULL) return 0;
    if (heap->size < heap->capacity) return 1;

    if (heap->capacity > INT_MAX / 2) return 0; /* overflow guard */
    int new_capacity = heap->capacity * 2;
    Value **new_data = realloc(heap->data, (size_t)new_capacity * sizeof(Value *));
    if (new_data == NULL) return 0;

    heap->data = new_data;
    heap->capacity = new_capacity;
    return 1;
}

static void _swap(Value **a, Value **b) {
    Value *tmp = *a;
    *a = *b;
    *b = tmp;
}

static void _bubble_up(Heap *heap, int i) {
    while (i > 0) {
        int p = _parent(i);
        if (_heap_cmp(heap, i, p) > 0) {
            _swap(&heap->data[i], &heap->data[p]);
            i = p;
        } else {
            break;
        }
    }
}

static void _bubble_down(Heap *heap, int i) {
    while (1) {
        int left = _left_child(i);
        int right = _right_child(i);
        int target = i;

        if (left < heap->size && _heap_cmp(heap, left, target) > 0) {
            target = left;
        }
        if (right < heap->size && _heap_cmp(heap, right, target) > 0) {
            target = right;
        }

        if (target == i) break;

        _swap(&heap->data[i], &heap->data[target]);
        i = target;
    }
}

static void _heap_insert_value(Heap *heap, Value *value) {
    if (heap == NULL || value == NULL) {
        value_free(value);
        return;
    }

    if (!_ensure_capacity(heap)) {
        value_free(value);
        return;
    }

    heap->data[heap->size] = value;
    heap->size++;
    _bubble_up(heap, heap->size - 1);
}

Heap *Heap_new(int is_max) {
    Heap *heap = malloc(sizeof(Heap));
    if (heap == NULL) return NULL;

    heap->capacity = 8;
    heap->size = 0;
    heap->is_max = is_max ? 1 : 0;
    heap->data = malloc((size_t)heap->capacity * sizeof(Value *));
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }

    return heap;
}

void Heap_destroy(Heap *heap) {
    if (heap == NULL) return;
    heap_clear(heap);
    free(heap->data);
    free(heap);
}

void _heap_insert_int(Heap *heap, int val) {
    _heap_insert_value(heap, make_int(val));
}

void _heap_insert_double(Heap *heap, double val) {
    _heap_insert_value(heap, make_double(val));
}

void _heap_insert_char(Heap *heap, char val) {
    _heap_insert_value(heap, make_char(val));
}

void _heap_insert_string(Heap *heap, const char *val) {
    _heap_insert_value(heap, make_string(val));
}

Value *heap_extract(Heap *heap) {
    if (heap == NULL || heap->size == 0) {
        return NULL;
    }

    Value *root = heap->data[0];

    heap->size--;
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heap->data[heap->size] = NULL;
        _bubble_down(heap, 0);
    } else {
        heap->data[0] = NULL;
    }

    return root; // caller owns this value — must call value_free() when done
}

Value *heap_peek(const Heap *heap) {
    if (heap == NULL || heap->size == 0) return NULL;
    return heap->data[0];
}

void heap_heapify(Heap *heap, Value **arr, int n) {
    if (heap == NULL || arr == NULL || n <= 0) {
        if (heap != NULL) heap_clear(heap);
        return;
    }

    heap_clear(heap);

    for (int i = 0; i < n; i++) {
        heap->size = i;
        if (!_ensure_capacity(heap)) {
            for (int j = i; j < n; j++) {
                value_free(arr[j]);
            }
            return;
        }
        heap->data[i] = arr[i];
    }
    heap->size = n;

    for (int i = (n / 2) - 1; i >= 0; i--) {
        _bubble_down(heap, i);
    }
}

int heap_size(const Heap *heap) {
    if (heap == NULL) return 0;
    return heap->size;
}

int heap_is_empty(const Heap *heap) {
    if (heap == NULL) return 1;
    return heap->size == 0;
}

void heap_clear(Heap *heap) {
    if (heap == NULL) return;

    for (int i = 0; i < heap->size; i++) {
        value_free(heap->data[i]);
        heap->data[i] = NULL;
    }
    heap->size = 0;
}

void heap_print(const Heap *heap) {
    if (heap == NULL) {
        printf("[]\n");
        return;
    }
    printf("[");
    for (int i = 0; i < heap->size; i++) {
        value_print(heap->data[i]);
        if (i < heap->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}
