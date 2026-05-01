#ifndef BST_H
#define BST_H

#include <stddef.h>
#include <stdint.h>

typedef struct BSTNode {
    void           *data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

typedef struct {
    BSTNode *root;
    size_t   size;
    int  (*cmp)(const void *, const void *);
    void (*print_fn)(const void *);
    void (*free_fn)(void *);
} BST;

// Create a new empty BST
BST* bst_create(int (*cmp)(const void *, const void *),
                void (*print_fn)(const void *),
                void (*free_fn)(void *));

// Destroy the BST and free all memory
void bst_destroy(BST *tree);

// Insert a new element into the BST
int bst_insert(BST *tree, void *data);

// Remove an element from the BST
int bst_remove(BST *tree, const void *data);

// Search for an element in the BST
BSTNode* bst_search(BST *tree, const void *data);

// Find the minimum element in the BST
BSTNode* bst_find_min(BST *tree);

// Find the maximum element in the BST
BSTNode* bst_find_max(BST *tree);

// Get the number of elements in the BST
size_t bst_size(BST *tree);

// Check if the BST is empty
int bst_is_empty(BST *tree);

// Remove all elements (calls free_fn if provided)
void bst_clear(BST *tree);

// Print all elements in-order using print_fn
void bst_print_inorder(BST *tree);

// Print all elements pre-order using print_fn
void bst_print_preorder(BST *tree);

// Print all elements post-order using print_fn
void bst_print_postorder(BST *tree);

#endif // BST_H
