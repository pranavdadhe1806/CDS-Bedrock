#include "../include/bedrock_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INITIAL_CAPACITY 4

// Internal helper to normalize buffer (move elements to start of array)
// This simplifies resizing by ensuring elements are contiguous
static int _normalize_buffer(BRQueue *queue) {
    if (queue == NULL) return 0;
    if (queue->head == 0) return 1;
    
    Value **new_data = malloc(queue->capacity * sizeof(Value*));
    if (new_data == NULL) return 0;
    
    // Copy elements in order: head to end, then 0 to tail
    int j = 0;
    for (int i = 0; i < queue->size; i++) {
        int idx = (queue->head + i) % queue->capacity;
        new_data[j++] = queue->data[idx];
    }
    
    free(queue->data);
    queue->data = new_data;
    queue->head = 0;
    queue->tail = queue->size;
    return 1;
}

// Internal helper to ensure capacity (double when full)
static int _ensure_capacity(BRQueue *queue) {
    if (queue == NULL) return 0;
    if (queue->size < queue->capacity) return 1;
    
    // Normalize before resizing to make elements contiguous
    if (!_normalize_buffer(queue)) return 0;
    
    int new_capacity = queue->capacity * 2;
    Value **new_data = realloc(queue->data, new_capacity * sizeof(Value*));
    if (new_data == NULL) return 0;
    
    queue->data = new_data;
    queue->capacity = new_capacity;
    queue->tail = queue->size;
    return 1;
}

BRQueue* BRQueue_new(void) {
    BRQueue *queue = malloc(sizeof(BRQueue));
    if (queue == NULL) return NULL;
    
    queue->data = malloc(INITIAL_CAPACITY * sizeof(Value*));
    if (queue->data == NULL) {
        free(queue);
        return NULL;
    }
    
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
    queue->capacity = INITIAL_CAPACITY;
    return queue;
}

void BRQueue_destroy(BRQueue *queue) {
    if (queue == NULL) return;
    brqueue_clear(queue);
    free(queue->data);
    free(queue);
}

// Type-specific enqueue implementations
void _brqueue_enqueue_int(BRQueue *queue, int val) {
    if (!_ensure_capacity(queue)) return;
    Value *v = make_int(val);
    if (v == NULL) return;
    queue->data[queue->tail] = v;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;
}

void _brqueue_enqueue_double(BRQueue *queue, double val) {
    if (!_ensure_capacity(queue)) return;
    Value *v = make_double(val);
    if (v == NULL) return;
    queue->data[queue->tail] = v;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;
}

void _brqueue_enqueue_char(BRQueue *queue, char val) {
    if (!_ensure_capacity(queue)) return;
    Value *v = make_char(val);
    if (v == NULL) return;
    queue->data[queue->tail] = v;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;
}

void _brqueue_enqueue_string(BRQueue *queue, const char *val) {
    if (!_ensure_capacity(queue)) return;
    Value *v = make_string(val);
    if (v == NULL) return;
    queue->data[queue->tail] = v;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->size++;
}

Value* brqueue_dequeue(BRQueue *queue) {
    if (queue == NULL || queue->size == 0) return NULL;
    
    Value *value = queue->data[queue->head];
    queue->data[queue->head] = NULL;
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;
    return value;
}

Value* brqueue_peek(BRQueue *queue) {
    if (queue == NULL || queue->size == 0) return NULL;
    return queue->data[queue->head];
}

int brqueue_is_empty(BRQueue *queue) {
    if (queue == NULL) return 1;
    return queue->size == 0;
}

int brqueue_size(BRQueue *queue) {
    if (queue == NULL) return 0;
    return queue->size;
}

void brqueue_clear(BRQueue *queue) {
    if (queue == NULL) return;
    
    for (int i = 0; i < queue->size; i++) {
        int idx = (queue->head + i) % queue->capacity;
        value_free(queue->data[idx]);
        queue->data[idx] = NULL;
    }
    
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
}

void brqueue_print(BRQueue *queue) {
    if (queue == NULL || queue->size == 0) {
        printf("Front: [] :Back\n");
        return;
    }
    
    printf("Front: [");
    for (int i = 0; i < queue->size; i++) {
        int idx = (queue->head + i) % queue->capacity;
        value_print(queue->data[idx]);
        if (i < queue->size - 1) {
            printf(", ");
        }
    }
    printf("] :Back\n");
}
