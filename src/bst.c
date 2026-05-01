#include "../include/bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * Binary Search Tree (BST)
 * 
 * A binary search tree that stores void* pointers with ordering.
 * 
 * Time Complexities (average case):
 * - Insert: O(log n)
 * - Remove: O(log n)
 * - Search: O(log n)
 * - Find Min/Max: O(log n)
 * 
 * Time Complexities (worst case - degenerate tree):
 * - Insert: O(n)
 * - Remove: O(n)
 * - Search: O(n)
 */

BST* bst_create(int (*cmp)(const void *, const void *),
                void (*print_fn)(const void *),
                void (*free_fn)(void *)) {
    (void)cmp;
    (void)print_fn;
    (void)free_fn;
    // TODO: implement
    return NULL;
}

void bst_destroy(BST *tree) {
    (void)tree;
    // TODO: implement
}

int bst_insert(BST *tree, void *data) {
    (void)tree;
    (void)data;
    // TODO: implement
    return 0;
}

int bst_remove(BST *tree, const void *data) {
    (void)tree;
    (void)data;
    // TODO: implement
    return 0;
}

BSTNode* bst_search(BST *tree, const void *data) {
    (void)tree;
    (void)data;
    // TODO: implement
    return NULL;
}

BSTNode* bst_find_min(BST *tree) {
    (void)tree;
    // TODO: implement
    return NULL;
}

BSTNode* bst_find_max(BST *tree) {
    (void)tree;
    // TODO: implement
    return NULL;
}

size_t bst_size(BST *tree) {
    (void)tree;
    // TODO: implement
    return 0;
}

int bst_is_empty(BST *tree) {
    (void)tree;
    // TODO: implement
    return 0;
}

void bst_clear(BST *tree) {
    (void)tree;
    // TODO: implement
}

void bst_print_inorder(BST *tree) {
    (void)tree;
    // TODO: implement
}

void bst_print_preorder(BST *tree) {
    (void)tree;
    // TODO: implement
}

void bst_print_postorder(BST *tree) {
    (void)tree;
    // TODO: implement
}
