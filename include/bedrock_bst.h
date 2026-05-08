#ifndef BEDROCK_BST_H
#define BEDROCK_BST_H

#include "value.h"

typedef struct BRBSTNode {
    Value             *data;
    struct BRBSTNode *left;
    struct BRBSTNode *right;
} BRBSTNode;

typedef struct {
    BRBSTNode *root;
    int        size;
} BRBST;

BRBST    *BRBST_new(void);
void      BRBST_destroy(BRBST *tree);
void      _brbst_insert_int(BRBST *tree, int val);
void      _brbst_insert_double(BRBST *tree, double val);
void      _brbst_insert_char(BRBST *tree, char val);
void      _brbst_insert_string(BRBST *tree, const char *val);
int       brbst_search(BRBST *tree, Value *val);
void      brbst_remove(BRBST *tree, Value *val);
Value     *brbst_find_min(BRBST *tree);
Value     *brbst_find_max(BRBST *tree);
int       brbst_height(BRBST *tree);
void      brbst_inorder(BRBST *tree);
void      brbst_preorder(BRBST *tree);
void      brbst_postorder(BRBST *tree);
int       brbst_size(BRBST *tree);
int       brbst_is_empty(BRBST *tree);
void      brbst_clear(BRBST *tree);

#endif // BEDROCK_BST_H
