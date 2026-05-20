#include "../include/bedrock_list.h"
#include <stdio.h>
#include <string.h>

LinkedList *LinkedList_new(void) {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (list == NULL) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void LinkedList_destroy(LinkedList *list) {
    if (list == NULL) return;
    list_clear(list);
    free(list);
}

// Helper to create a new BRNode with given Value
static BRNode *_create_node(Value *val) {
    if (val == NULL) return NULL;
    BRNode *node = malloc(sizeof(BRNode));
    if (node == NULL) {
        value_free(val);
        return NULL;
    }
    node->data = val;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/* ===================================================================
 * Core push/insert/update helpers — single implementation per operation.
 * Type-specific wrappers below just call make_*() then delegate here.
 * =================================================================== */

static void _list_push_front_value(LinkedList *list, Value *v) {
    BRNode *node = _create_node(v);
    if (node == NULL) return;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }
    list->size++;
}

static void _list_push_back_value(LinkedList *list, Value *v) {
    BRNode *node = _create_node(v);
    if (node == NULL) return;

    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
}

static void _list_insert_at_value(LinkedList *list, int index, Value *v) {
    /* Boundary cases delegate to push_front/push_back */
    if (index == 0) {
        _list_push_front_value(list, v);
        return;
    }
    if (index == list->size) {
        _list_push_back_value(list, v);
        return;
    }

    BRNode *node = _create_node(v);
    if (node == NULL) return;

    BRNode *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    node->prev = current->prev;
    node->next = current;
    current->prev->next = node;
    current->prev = node;
    list->size++;
}

static void _list_update_at_value(LinkedList *list, int index, Value *v) {
    if (v == NULL) return;

    BRNode *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    value_free(current->data);
    current->data = v;
}

/* ===================================================================
 * Type-specific wrappers — push_front
 * =================================================================== */

void _list_push_front_int(LinkedList *list, int val) {
    if (list == NULL) return;
    _list_push_front_value(list, make_int(val));
}

void _list_push_front_double(LinkedList *list, double val) {
    if (list == NULL) return;
    _list_push_front_value(list, make_double(val));
}

void _list_push_front_char(LinkedList *list, char val) {
    if (list == NULL) return;
    _list_push_front_value(list, make_char(val));
}

void _list_push_front_string(LinkedList *list, const char *val) {
    if (list == NULL) return;
    _list_push_front_value(list, make_string(val));
}

/* ===================================================================
 * Type-specific wrappers — push_back
 * =================================================================== */

void _list_push_back_int(LinkedList *list, int val) {
    if (list == NULL) return;
    _list_push_back_value(list, make_int(val));
}

void _list_push_back_double(LinkedList *list, double val) {
    if (list == NULL) return;
    _list_push_back_value(list, make_double(val));
}

void _list_push_back_char(LinkedList *list, char val) {
    if (list == NULL) return;
    _list_push_back_value(list, make_char(val));
}

void _list_push_back_string(LinkedList *list, const char *val) {
    if (list == NULL) return;
    _list_push_back_value(list, make_string(val));
}

/* ===================================================================
 * Type-specific wrappers — insert_at
 * =================================================================== */

void _list_insert_at_int(LinkedList *list, int index, int val) {
    if (list == NULL || index < 0 || index > list->size) return;
    _list_insert_at_value(list, index, make_int(val));
}

void _list_insert_at_double(LinkedList *list, int index, double val) {
    if (list == NULL || index < 0 || index > list->size) return;
    _list_insert_at_value(list, index, make_double(val));
}

void _list_insert_at_char(LinkedList *list, int index, char val) {
    if (list == NULL || index < 0 || index > list->size) return;
    _list_insert_at_value(list, index, make_char(val));
}

void _list_insert_at_string(LinkedList *list, int index, const char *val) {
    if (list == NULL || index < 0 || index > list->size) return;
    _list_insert_at_value(list, index, make_string(val));
}

/* ===================================================================
 * Type-specific wrappers — update_at
 * =================================================================== */

void _list_update_at_int(LinkedList *list, int index, int val) {
    if (list == NULL || index < 0 || index >= list->size) return;
    _list_update_at_value(list, index, make_int(val));
}

void _list_update_at_double(LinkedList *list, int index, double val) {
    if (list == NULL || index < 0 || index >= list->size) return;
    _list_update_at_value(list, index, make_double(val));
}

void _list_update_at_char(LinkedList *list, int index, char val) {
    if (list == NULL || index < 0 || index >= list->size) return;
    _list_update_at_value(list, index, make_char(val));
}

void _list_update_at_string(LinkedList *list, int index, const char *val) {
    if (list == NULL || index < 0 || index >= list->size) return;
    _list_update_at_value(list, index, make_string(val));
}

/* ===================================================================
 * Peek / Pop / Get / Remove — unchanged
 * =================================================================== */

Value *list_peek_front(LinkedList *list) {
    if (list == NULL || list->head == NULL) return NULL;
    return list->head->data;
}

Value *list_peek_back(LinkedList *list) {
    if (list == NULL || list->tail == NULL) return NULL;
    return list->tail->data;
}

Value *list_pop_front(LinkedList *list) {
    if (list == NULL || list->head == NULL) return NULL;
    
    BRNode *node = list->head;
    Value *value = node->data;
    
    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = node->next;
        list->head->prev = NULL;
    }
    
    free(node);
    list->size--;
    return value;
}

Value *list_pop_back(LinkedList *list) {
    if (list == NULL || list->tail == NULL) return NULL;
    
    BRNode *node = list->tail;
    Value *value = node->data;
    
    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = node->prev;
        list->tail->next = NULL;
    }
    
    free(node);
    list->size--;
    return value;
}

Value *list_get_at(LinkedList *list, int index) {
    if (list == NULL || index < 0 || index >= list->size) return NULL;
    
    BRNode *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

void list_remove_at(LinkedList *list, int index) {
    if (list == NULL || index < 0 || index >= list->size) return;
    
    if (index == 0) {
        Value *v = list_pop_front(list);
        value_free(v);
        return;
    }
    if (index == list->size - 1) {
        Value *v = list_pop_back(list);
        value_free(v);
        return;
    }
    
    BRNode *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    value_free(current->data);
    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current);
    list->size--;
}

/* ===================================================================
 * Utility — size, contains, reverse, clear, print
 * =================================================================== */

int list_size(LinkedList *list) {
    if (list == NULL) return 0;
    return list->size;
}

int _list_contains_int(LinkedList *list, int val) {
    if (list == NULL) return 0;
    Value tmp = {TYPE_INT, {.i = val}};
    BRNode *current = list->head;
    while (current != NULL) {
        if (value_equals(current->data, &tmp)) return 1;
        current = current->next;
    }
    return 0;
}

int _list_contains_double(LinkedList *list, double val) {
    if (list == NULL) return 0;
    Value tmp = {TYPE_DOUBLE, {.d = val}};
    BRNode *current = list->head;
    while (current != NULL) {
        if (value_equals(current->data, &tmp)) return 1;
        current = current->next;
    }
    return 0;
}

int _list_contains_char(LinkedList *list, char val) {
    if (list == NULL) return 0;
    Value tmp = {TYPE_CHAR, {.c = val}};
    BRNode *current = list->head;
    while (current != NULL) {
        if (value_equals(current->data, &tmp)) return 1;
        current = current->next;
    }
    return 0;
}

int _list_contains_string(LinkedList *list, const char *val) {
    if (list == NULL || val == NULL) return 0;
    Value tmp = {TYPE_STRING, {.s = (char*)val}};
    BRNode *current = list->head;
    while (current != NULL) {
        if (value_equals(current->data, &tmp)) return 1;
        current = current->next;
    }
    return 0;
}

void list_reverse(LinkedList *list) {
    if (list == NULL || list->size <= 1) return;
    
    BRNode *current = list->head;
    BRNode *temp = NULL;
    
    while (current != NULL) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }
    
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

void list_clear(LinkedList *list) {
    if (list == NULL) return;
    
    BRNode *current = list->head;
    while (current != NULL) {
        BRNode *next = current->next;
        value_free(current->data);
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void list_print(LinkedList *list) {
    if (list == NULL || list->head == NULL) {
        printf("NULL\n");
        return;
    }
    
    BRNode *current = list->head;
    while (current != NULL) {
        printf("[");
        value_print(current->data);
        printf("]");
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" -> NULL\n");
}
