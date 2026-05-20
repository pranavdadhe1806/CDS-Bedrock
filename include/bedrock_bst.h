#ifndef BEDROCK_BST_H
#define BEDROCK_BST_H

#include "value.h"

typedef struct BRBSTNode {
    Value             *data;
    struct BRBSTNode *left;
    struct BRBSTNode *right;
    int               height;
} BRBSTNode;

typedef struct {
    BRBSTNode *root;
    int        size;
} BRBST;

/* Lifecycle */
BRBST    *BRBST_new(void);                              /* caller owns */
void      BRBST_destroy(BRBST *tree);                   /* frees tree + all contained Values */

/* Type-specific insert — tree takes ownership of created Value */
void      _brbst_insert_int(BRBST *tree, int val);
void      _brbst_insert_double(BRBST *tree, double val);
void      _brbst_insert_char(BRBST *tree, char val);
void      _brbst_insert_string(BRBST *tree, const char *val);

int       brbst_search(const BRBST *tree, Value *val);        /* borrows val for comparison */
void      brbst_remove(BRBST *tree, Value *val);        /* borrows val; frees matched node’s Value internally */

/* Returns BORROWED pointer; caller must NOT free */
Value     *brbst_find_min(const BRBST *tree);
Value     *brbst_find_max(const BRBST *tree);

int       brbst_height(const BRBST *tree);
void      brbst_inorder(const BRBST *tree);
void      brbst_preorder(const BRBST *tree);
void      brbst_postorder(const BRBST *tree);
int       brbst_size(const BRBST *tree);
int       brbst_is_empty(const BRBST *tree);
void      brbst_clear(BRBST *tree);                     /* frees all contained Values */

#endif // BEDROCK_BST_H
