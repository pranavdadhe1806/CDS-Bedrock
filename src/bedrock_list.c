#include "../include/bedrock_list.h"
#include <stdio.h>
#include <string.h>

LinkedList *LinkedList_new(void) {
    LinkedList *list = malloc(sizeof(LinkedList));
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
    BRNode *node = malloc(sizeof(BRNode));
    node->data = val;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

// Push Front implementations
void _list_push_front_int(LinkedList *list, int val) {
    Value *v = make_int(val);
    BRNode *node = _create_node(v);
    
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

void _list_push_front_double(LinkedList *list, double val) {
    Value *v = make_double(val);
    BRNode *node = _create_node(v);
    
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

void _list_push_front_char(LinkedList *list, char val) {
    Value *v = make_char(val);
    BRNode *node = _create_node(v);
    
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

void _list_push_front_string(LinkedList *list, const char *val) {
    Value *v = make_string(val);
    BRNode *node = _create_node(v);
    
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

// Push Back implementations
void _list_push_back_int(LinkedList *list, int val) {
    Value *v = make_int(val);
    BRNode *node = _create_node(v);
    
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

void _list_push_back_double(LinkedList *list, double val) {
    Value *v = make_double(val);
    BRNode *node = _create_node(v);
    
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

void _list_push_back_char(LinkedList *list, char val) {
    Value *v = make_char(val);
    BRNode *node = _create_node(v);
    
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

void _list_push_back_string(LinkedList *list, const char *val) {
    Value *v = make_string(val);
    BRNode *node = _create_node(v);
    
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

// Peek operations
Value *list_peek_front(LinkedList *list) {
    if (list == NULL || list->head == NULL) return NULL;
    return list->head->data;
}

Value *list_peek_back(LinkedList *list) {
    if (list == NULL || list->tail == NULL) return NULL;
    return list->tail->data;
}

// Pop Front
void list_pop_front(LinkedList *list) { // Free internally to prevent memory leaks
    if (list == NULL || list->head == NULL) return;
    
    BRNode *node = list->head;
    value_free(node->data); // Free the Value before removing node
    
    if (list->head == list->tail) {
        // Only one node
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = node->next;
        list->head->prev = NULL;
    }
    
    free(node);
    list->size--;
}

// Pop Back
void list_pop_back(LinkedList *list) { // Free internally to prevent memory leaks
    if (list == NULL || list->tail == NULL) return;
    
    BRNode *node = list->tail;
    value_free(node->data); // Free the Value before removing node
    
    if (list->head == list->tail) {
        // Only one node
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = node->prev;
        list->tail->next = NULL;
    }
    
    free(node);
    list->size--;
}

// Get At
Value *list_get_at(LinkedList *list, int index) {
    if (list == NULL || index < 0 || index >= list->size) return NULL;
    
    BRNode *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

// Insert At implementations
void _list_insert_at_int(LinkedList *list, int index, int val) {
    if (index < 0 || index > list->size) return;
    if (index == 0) {
        _list_push_front_int(list, val);
        return;
    }
    if (index == list->size) {
        _list_push_back_int(list, val);
        return;
    }
    
    Value *v = make_int(val);
    BRNode *node = _create_node(v);
    
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

void _list_insert_at_double(LinkedList *list, int index, double val) {
    if (index < 0 || index > list->size) return;
    if (index == 0) {
        _list_push_front_double(list, val);
        return;
    }
    if (index == list->size) {
        _list_push_back_double(list, val);
        return;
    }
    
    Value *v = make_double(val);
    BRNode *node = _create_node(v);
    
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

void _list_insert_at_char(LinkedList *list, int index, char val) {
    if (index < 0 || index > list->size) return;
    if (index == 0) {
        _list_push_front_char(list, val);
        return;
    }
    if (index == list->size) {
        _list_push_back_char(list, val);
        return;
    }
    
    Value *v = make_char(val);
    BRNode *node = _create_node(v);
    
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

void _list_insert_at_string(LinkedList *list, int index, const char *val) {
    if (index < 0 || index > list->size) return;
    if (index == 0) {
        _list_push_front_string(list, val);
        return;
    }
    if (index == list->size) {
        _list_push_back_string(list, val);
        return;
    }
    
    Value *v = make_string(val);
    BRNode *node = _create_node(v);
    
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

// Remove At
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

// Update At implementations
void _list_update_at_int(LinkedList *list, int index, int val) {
    if (list == NULL || index < 0 || index >= list->size) return;
    
    BRNode *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    value_free(current->data);
    current->data = make_int(val);
}

void _list_update_at_double(LinkedList *list, int index, double val) {
    if (list == NULL || index < 0 || index >= list->size) return;
    
    BRNode *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    value_free(current->data);
    current->data = make_double(val);
}

void _list_update_at_char(LinkedList *list, int index, char val) {
    if (list == NULL || index < 0 || index >= list->size) return;
    
    BRNode *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    value_free(current->data);
    current->data = make_char(val);
}

void _list_update_at_string(LinkedList *list, int index, const char *val) {
    if (list == NULL || index < 0 || index >= list->size) return;
    
    BRNode *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    value_free(current->data);
    current->data = make_string(val);
}

// Size
int list_size(LinkedList *list) {
    if (list == NULL) return 0;
    return list->size;
}

// Contains implementations
int _list_contains_int(LinkedList *list, int val) {
    Value tmp = {TYPE_INT, {.i = val}};
    BRNode *current = list->head;
    while (current != NULL) {
        if (value_equals(current->data, &tmp)) return 1;
        current = current->next;
    }
    return 0;
}

int _list_contains_double(LinkedList *list, double val) {
    Value tmp = {TYPE_DOUBLE, {.d = val}};
    BRNode *current = list->head;
    while (current != NULL) {
        if (value_equals(current->data, &tmp)) return 1;
        current = current->next;
    }
    return 0;
}

int _list_contains_char(LinkedList *list, char val) {
    Value tmp = {TYPE_CHAR, {.c = val}};
    BRNode *current = list->head;
    while (current != NULL) {
        if (value_equals(current->data, &tmp)) return 1;
        current = current->next;
    }
    return 0;
}

int _list_contains_string(LinkedList *list, const char *val) {
    Value tmp = {TYPE_STRING, {.s = (char*)val}};
    BRNode *current = list->head;
    while (current != NULL) {
        if (value_equals(current->data, &tmp)) return 1;
        current = current->next;
    }
    return 0;
}

// Reverse
void list_reverse(LinkedList *list) {
    if (list == NULL || list->size <= 1) return;
    
    BRNode *current = list->head;
    BRNode *temp = NULL;
    
    // Swap prev and next for all nodes
    while (current != NULL) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;  // Move to next (which was prev)
    }
    
    // Swap head and tail
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

// Clear
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

// Print
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
