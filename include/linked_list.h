#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>
#include <stdint.h>

typedef struct Node {
    void        *data;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    Node   *head;
    Node   *tail;
    size_t  size;
    int  (*cmp)(const void *, const void *);
    void (*print_fn)(const void *);
    void (*free_fn)(void *);
} LinkedList;

// Create a new empty linked list
LinkedList* list_create(int (*cmp)(const void *, const void *),
                        void (*print_fn)(const void *),
                        void (*free_fn)(void *));

// Destroy the list and free all memory
void list_destroy(LinkedList *list);

// Add an element to the front of the list
int list_push_front(LinkedList *list, void *data);

// Add an element to the back of the list
int list_push_back(LinkedList *list, void *data);

// Remove and return the front element
void* list_pop_front(LinkedList *list);

// Remove and return the back element
void* list_pop_back(LinkedList *list);

// Insert an element after a specific node
int list_insert_after(LinkedList *list, Node *node, void *data);

// Insert an element before a specific node
int list_insert_before(LinkedList *list, Node *node, void *data);

// Remove a specific node from the list
int list_remove_node(LinkedList *list, Node *node);

// Search for an element using the comparator
Node* list_find(LinkedList *list, const void *data);

// Remove all elements (calls free_fn if provided)
void list_clear(LinkedList *list);

// Print all elements using print_fn
void list_print(LinkedList *list);

// Get the first node
Node* list_front(LinkedList *list);

// Get the last node
Node* list_back(LinkedList *list);

// Get the size of the list
size_t list_size(LinkedList *list);

#endif // LINKED_LIST_H
