#include "../include/bedrock_bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static BRBSTNode *_insert_recursive(BRBSTNode *node, Value *val, int *created) {
    if (node == NULL) {
        BRBSTNode *new_node = malloc(sizeof(BRBSTNode));
        if (new_node == NULL) {
            value_free(val);
            *created = 0;
            return NULL;
        }
        new_node->data = val;
        new_node->left = NULL;
        new_node->right = NULL;
        *created = 1;
        return new_node;
    }

    int cmp = value_compare(val, node->data);
    if (cmp < 0) {
        node->left = _insert_recursive(node->left, val, created);
    } else if (cmp > 0) {
        node->right = _insert_recursive(node->right, val, created);
    } else {
        value_free(val);
        *created = 0;
    }

    return node;
}

static BRBSTNode *_search_recursive(BRBSTNode *node, Value *val) {
    if (node == NULL) return NULL;

    int cmp = value_compare(val, node->data);
    if (cmp < 0) {
        return _search_recursive(node->left, val);
    } else if (cmp > 0) {
        return _search_recursive(node->right, val);
    } else {
        return node;
    }
}

static BRBSTNode *_find_min_node(BRBSTNode *node) {
    if (node == NULL) return NULL;
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

static BRBSTNode *_detach_min_node(BRBSTNode *node, Value **detached_data) {
    if (node == NULL) return NULL;

    if (node->left == NULL) {
        BRBSTNode *right = node->right;
        *detached_data = node->data;
        node->data = NULL;
        free(node);
        return right;
    }

    node->left = _detach_min_node(node->left, detached_data);
    return node;
}

static BRBSTNode *_remove_recursive(BRBSTNode *node, Value *val, int *removed) {
    if (node == NULL) {
        *removed = 0;
        return NULL;
    }

    int cmp = value_compare(val, node->data);
    if (cmp < 0) {
        node->left = _remove_recursive(node->left, val, removed);
    } else if (cmp > 0) {
        node->right = _remove_recursive(node->right, val, removed);
    } else {
        *removed = 1;

        if (node->left == NULL && node->right == NULL) {
            value_free(node->data);
            free(node);
            return NULL;
        } else if (node->left == NULL) {
            BRBSTNode *temp = node->right;
            value_free(node->data);
            free(node);
            return temp;
        } else if (node->right == NULL) {
            BRBSTNode *temp = node->left;
            value_free(node->data);
            free(node);
            return temp;
        } else {
            value_free(node->data);
            Value *successor_data = NULL;
            node->right = _detach_min_node(node->right, &successor_data);
            node->data = successor_data;
        }
    }

    return node;
}

static int _height(BRBSTNode *node) {
    if (node == NULL) return 0;
    int left_height = _height(node->left);
    int right_height = _height(node->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

static void _inorder(BRBSTNode *node) {
    if (node == NULL) return;
    _inorder(node->left);
    value_print(node->data);
    printf(" ");
    _inorder(node->right);
}

static void _preorder(BRBSTNode *node) {
    if (node == NULL) return;
    value_print(node->data);
    printf(" ");
    _preorder(node->left);
    _preorder(node->right);
}

static void _postorder(BRBSTNode *node) {
    if (node == NULL) return;
    _postorder(node->left);
    _postorder(node->right);
    value_print(node->data);
    printf(" ");
}

static void _destroy_iterative(BRBSTNode *root) {
    if (root == NULL) return;

    /* Explicit stack to avoid recursion (prevents stack overflow on O(n)-depth trees) */
    int cap = 32;
    int top = 0;
    BRBSTNode **stack = malloc((size_t)cap * sizeof(BRBSTNode *));
    if (stack == NULL) {
        /* Fallback: if we can't even allocate a small stack, do a simple
         * iterative post-order using Morris-like traversal isn't easy,
         * so just recurse as before (better than leaking everything). */
        if (root->left)  _destroy_iterative(root->left);
        if (root->right) _destroy_iterative(root->right);
        value_free(root->data);
        free(root);
        return;
    }

    stack[top++] = root;

    while (top > 0) {
        BRBSTNode *node = stack[--top];

        /* Push children before freeing (pre-order push, post-order free via stack) */
        if (node->left != NULL) {
            if (top >= cap) {
                int new_cap = cap * 2;
                BRBSTNode **tmp = realloc(stack, (size_t)new_cap * sizeof(BRBSTNode *));
                if (tmp != NULL) { stack = tmp; cap = new_cap; }
            }
            if (top < cap) stack[top++] = node->left;
        }
        if (node->right != NULL) {
            if (top >= cap) {
                int new_cap = cap * 2;
                BRBSTNode **tmp = realloc(stack, (size_t)new_cap * sizeof(BRBSTNode *));
                if (tmp != NULL) { stack = tmp; cap = new_cap; }
            }
            if (top < cap) stack[top++] = node->right;
        }

        value_free(node->data);
        free(node);
    }

    free(stack);
}

BRBST *BRBST_new(void) {
    BRBST *tree = malloc(sizeof(BRBST));
    if (tree == NULL) return NULL;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void BRBST_destroy(BRBST *tree) {
    if (tree == NULL) return;
    _destroy_iterative(tree->root);
    free(tree);
}

void _brbst_insert_int(BRBST *tree, int val) {
    if (tree == NULL) return;
    Value *v = make_int(val);
    if (v == NULL) return;
    int created = 0;
    tree->root = _insert_recursive(tree->root, v, &created);
    if (created) tree->size++;
}

void _brbst_insert_double(BRBST *tree, double val) {
    if (tree == NULL) return;
    Value *v = make_double(val);
    if (v == NULL) return;
    int created = 0;
    tree->root = _insert_recursive(tree->root, v, &created);
    if (created) tree->size++;
}

void _brbst_insert_char(BRBST *tree, char val) {
    if (tree == NULL) return;
    Value *v = make_char(val);
    if (v == NULL) return;
    int created = 0;
    tree->root = _insert_recursive(tree->root, v, &created);
    if (created) tree->size++;
}

void _brbst_insert_string(BRBST *tree, const char *val) {
    if (tree == NULL) return;
    Value *v = make_string(val);
    if (v == NULL) return;
    int created = 0;
    tree->root = _insert_recursive(tree->root, v, &created);
    if (created) tree->size++;
}

int brbst_search(BRBST *tree, Value *val) {
    if (tree == NULL || val == NULL) return 0;
    return _search_recursive(tree->root, val) != NULL;
}

void brbst_remove(BRBST *tree, Value *val) {
    if (tree == NULL || val == NULL) return;
    int removed = 0;
    tree->root = _remove_recursive(tree->root, val, &removed);
    if (removed) tree->size--;
}

Value *brbst_find_min(BRBST *tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    BRBSTNode *node = _find_min_node(tree->root);
    return node->data;
}

Value *brbst_find_max(BRBST *tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    BRBSTNode *node = tree->root;
    while (node->right != NULL) {
        node = node->right;
    }
    return node->data;
}

int brbst_height(BRBST *tree) {
    if (tree == NULL) return 0;
    return _height(tree->root);
}

void brbst_inorder(BRBST *tree) {
    if (tree == NULL) return;
    _inorder(tree->root);
    printf("\n");
}

void brbst_preorder(BRBST *tree) {
    if (tree == NULL) return;
    _preorder(tree->root);
    printf("\n");
}

void brbst_postorder(BRBST *tree) {
    if (tree == NULL) return;
    _postorder(tree->root);
    printf("\n");
}

int brbst_size(BRBST *tree) {
    if (tree == NULL) return 0;
    return tree->size;
}

int brbst_is_empty(BRBST *tree) {
    if (tree == NULL) return 1;
    return tree->size == 0;
}

void brbst_clear(BRBST *tree) {
    if (tree == NULL) return;
    _destroy_iterative(tree->root);
    tree->root = NULL;
    tree->size = 0;
}
