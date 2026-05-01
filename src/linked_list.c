#include "../include/linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * Doubly Linked List
 * 
 * A doubly-linked list that stores void* pointers.
 * 
 * Time Complexities:
 * - Access (by index): O(n)
 * - Push/Pop (front/back): O(1)
 * - Insert/Remove (given node): O(1)
 * - Search: O(n)
 */

LinkedList* list_create(int (*cmp)(const void *, const void *),
                        void (*print_fn)(const void *),
                        void (*free_fn)(void *)) {
    (void)cmp;
    (void)print_fn;
    (void)free_fn;
    // TODO: implement
    return NULL;
}

void list_destroy(LinkedList *list) {
    (void)list;
    // TODO: implement
}

int list_push_front(LinkedList *list, void *data) {
    (void)list;
    (void)data;
    // TODO: implement
    return 0;
}

int list_push_back(LinkedList *list, void *data) {
    (void)list;
    (void)data;
    // TODO: implement
    return 0;
}

void* list_pop_front(LinkedList *list) {
    (void)list;
    // TODO: implement
    return NULL;
}

void* list_pop_back(LinkedList *list) {
    (void)list;
    // TODO: implement
    return NULL;
}

int list_insert_after(LinkedList *list, Node *node, void *data) {
    (void)list;
    (void)node;
    (void)data;
    // TODO: implement
    return 0;
}

int list_insert_before(LinkedList *list, Node *node, void *data) {
    (void)list;
    (void)node;
    (void)data;
    // TODO: implement
    return 0;
}

int list_remove_node(LinkedList *list, Node *node) {
    (void)list;
    (void)node;
    // TODO: implement
    return 0;
}

Node* list_find(LinkedList *list, const void *data) {
    (void)list;
    (void)data;
    // TODO: implement
    return NULL;
}

void list_clear(LinkedList *list) {
    (void)list;
    // TODO: implement
}

void list_print(LinkedList *list) {
    (void)list;
    // TODO: implement
}

Node* list_front(LinkedList *list) {
    (void)list;
    // TODO: implement
    return NULL;
}

Node* list_back(LinkedList *list) {
    (void)list;
    // TODO: implement
    return NULL;
}

size_t list_size(LinkedList *list) {
    (void)list;
    // TODO: implement
    return 0;
}
