#include "../include/bedrock.h"
#include <stdio.h>
#include <stdlib.h>

static int pass_count = 0;
static int fail_count = 0;

#define TEST(name) printf("Testing: %s\n", name);

#define PASS() do { \
    printf("  PASS\n"); \
    pass_count++; \
} while(0)

#define FAIL(msg) do { \
    printf("  FAIL: %s\n", msg); \
    fail_count++; \
    return; \
} while(0)

#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        FAIL(msg); \
    } \
} while(0)

void test_insert_and_inorder(void) {
    TEST("insert_and_inorder");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    bst_insert(tree, 7);
    bst_insert(tree, 1);
    bst_insert(tree, 4);
    
    printf("  Inorder: ");
    bst_inorder(tree);
    ASSERT(bst_size(tree) == 5, "size should be 5");
    
    BRBST_destroy(tree);
    PASS();
}

void test_search_found(void) {
    TEST("search_found");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 10);
    
    ASSERT(bst_contains(tree, 10) == 1, "should find 10");
    
    BRBST_destroy(tree);
    PASS();
}

void test_search_not_found(void) {
    TEST("search_not_found");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 10);
    
    ASSERT(bst_contains(tree, 99) == 0, "should not find 99");
    
    BRBST_destroy(tree);
    PASS();
}

void test_duplicate_insert(void) {
    TEST("duplicate_insert");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 5);
    
    ASSERT(bst_size(tree) == 1, "size should stay 1 on duplicate");
    
    BRBST_destroy(tree);
    PASS();
}

void test_remove_leaf(void) {
    TEST("remove_leaf");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    bst_insert(tree, 7);
    bst_insert(tree, 1);
    bst_insert(tree, 4);
    
    bst_delete(tree, 1);
    ASSERT(bst_size(tree) == 4, "size should be 4 after removal");
    
    printf("  Inorder after remove 1: ");
    bst_inorder(tree);
    
    BRBST_destroy(tree);
    PASS();
}

void test_remove_one_child(void) {
    TEST("remove_one_child");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    bst_insert(tree, 7);
    bst_insert(tree, 4);
    
    bst_delete(tree, 3);
    ASSERT(bst_size(tree) == 3, "size should be 3 after removal");
    
    printf("  Inorder after remove 3: ");
    bst_inorder(tree);
    
    BRBST_destroy(tree);
    PASS();
}

void test_remove_two_children(void) {
    TEST("remove_two_children");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    bst_insert(tree, 7);
    
    bst_delete(tree, 5);
    ASSERT(bst_size(tree) == 2, "size should be 2 after removal");
    
    printf("  Inorder after remove 5: ");
    bst_inorder(tree);
    
    BRBST_destroy(tree);
    PASS();
}

void test_remove_root(void) {
    TEST("remove_root");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 10);
    
    bst_delete(tree, 10);
    ASSERT(bst_size(tree) == 0, "size should be 0 after removing root");
    ASSERT(bst_is_empty(tree) == 1, "tree should be empty");
    
    BRBST_destroy(tree);
    PASS();
}

void test_find_min(void) {
    TEST("find_min");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    bst_insert(tree, 7);
    
    Value *min = bst_find_min(tree);
    ASSERT(min != NULL, "min should not be NULL");
    ASSERT(min->as.i == 3, "min should be 3");
    
    BRBST_destroy(tree);
    PASS();
}

void test_find_max(void) {
    TEST("find_max");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    bst_insert(tree, 7);
    
    Value *max = bst_find_max(tree);
    ASSERT(max != NULL, "max should not be NULL");
    ASSERT(max->as.i == 7, "max should be 7");
    
    BRBST_destroy(tree);
    PASS();
}

void test_height_empty(void) {
    TEST("height_empty");
    BRBST *tree = BRBST_new();
    
    ASSERT(bst_height(tree) == 0, "empty tree height should be 0");
    
    BRBST_destroy(tree);
    PASS();
}

void test_height_single(void) {
    TEST("height_single");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    
    ASSERT(bst_height(tree) == 1, "single node height should be 1");
    
    BRBST_destroy(tree);
    PASS();
}

void test_height_known_shape(void) {
    TEST("height_known_shape");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    bst_insert(tree, 7);
    bst_insert(tree, 1);
    
    ASSERT(bst_height(tree) == 3, "height should be 3 for this shape");
    
    BRBST_destroy(tree);
    PASS();
}

void test_size_tracking(void) {
    TEST("size_tracking");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 1);
    bst_insert(tree, 2);
    bst_insert(tree, 3);
    bst_insert(tree, 4);
    bst_insert(tree, 5);
    
    ASSERT(bst_size(tree) == 5, "size should be 5 after 5 inserts");
    
    bst_delete(tree, 1);
    bst_delete(tree, 5);
    
    ASSERT(bst_size(tree) == 3, "size should be 3 after 2 removals");
    
    BRBST_destroy(tree);
    PASS();
}

void test_clear(void) {
    TEST("clear");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 1);
    bst_insert(tree, 2);
    bst_insert(tree, 3);
    bst_insert(tree, 4);
    bst_insert(tree, 5);
    
    bst_clear(tree);
    ASSERT(bst_size(tree) == 0, "size should be 0 after clear");
    ASSERT(bst_is_empty(tree) == 1, "tree should be empty after clear");
    
    BRBST_destroy(tree);
    PASS();
}

void test_mixed_types(void) {
    TEST("mixed_types");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 42);
    bst_insert(tree, "hello");
    
    ASSERT(bst_contains(tree, 42) == 1, "should find int 42");
    ASSERT(bst_contains(tree, "hello") == 1, "should find string hello");
    
    BRBST_destroy(tree);
    PASS();
}

void test_sorted_insert(void) {
    TEST("sorted_insert");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 1);
    bst_insert(tree, 2);
    bst_insert(tree, 3);
    bst_insert(tree, 4);
    bst_insert(tree, 5);
    
    ASSERT(bst_size(tree) == 5, "size should be 5");
    ASSERT(bst_height(tree) == 5, "degenerate tree height should be 5");
    
    printf("  Inorder (degenerate): ");
    bst_inorder(tree);
    
    BRBST_destroy(tree);
    PASS();
}

void test_destroy_valgrind(void) {
    TEST("destroy_valgrind");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 1);
    bst_insert(tree, 2);
    bst_insert(tree, 3);
    
    BRBST_destroy(tree);
    PASS();
}

void test_preorder_traversal(void) {
    TEST("preorder_traversal");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    bst_insert(tree, 7);
    
    printf("  Preorder: ");
    bst_preorder(tree);
    
    BRBST_destroy(tree);
    PASS();
}

void test_postorder_traversal(void) {
    TEST("postorder_traversal");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    bst_insert(tree, 7);
    
    printf("  Postorder: ");
    bst_postorder(tree);
    
    BRBST_destroy(tree);
    PASS();
}

void test_remove_nonexistent(void) {
    TEST("remove_nonexistent");
    BRBST *tree = BRBST_new();
    bst_insert(tree, 5);
    bst_insert(tree, 3);
    
    bst_delete(tree, 99);
    ASSERT(bst_size(tree) == 2, "size should remain 2 after removing nonexistent");
    
    BRBST_destroy(tree);
    PASS();
}

void test_find_min_empty(void) {
    TEST("find_min_empty");
    BRBST *tree = BRBST_new();
    
    Value *min = bst_find_min(tree);
    ASSERT(min == NULL, "min should be NULL for empty tree");
    
    BRBST_destroy(tree);
    PASS();
}

void test_find_max_empty(void) {
    TEST("find_max_empty");
    BRBST *tree = BRBST_new();
    
    Value *max = bst_find_max(tree);
    ASSERT(max == NULL, "max should be NULL for empty tree");
    
    BRBST_destroy(tree);
    PASS();
}

void test_null_tree_handling(void) {
    TEST("null_tree_handling");
    bst_insert(NULL, 5);
    bst_delete(NULL, 5);
    bst_contains(NULL, 5);
    bst_find_min(NULL);
    bst_find_max(NULL);
    bst_height(NULL);
    bst_size(NULL);
    bst_is_empty(NULL);
    bst_clear(NULL);
    bst_inorder(NULL);
    bst_preorder(NULL);
    bst_postorder(NULL);
    
    PASS();
}

int main(void) {
    printf("=== BST Tests ===\n\n");
    
    test_insert_and_inorder();
    test_search_found();
    test_search_not_found();
    test_duplicate_insert();
    test_remove_leaf();
    test_remove_one_child();
    test_remove_two_children();
    test_remove_root();
    test_find_min();
    test_find_max();
    test_height_empty();
    test_height_single();
    test_height_known_shape();
    test_size_tracking();
    test_clear();
    test_mixed_types();
    test_sorted_insert();
    test_destroy_valgrind();
    test_preorder_traversal();
    test_postorder_traversal();
    test_remove_nonexistent();
    test_find_min_empty();
    test_find_max_empty();
    test_null_tree_handling();
    
    printf("\n=== Summary ===\n");
    printf("Passed: %d\n", pass_count);
    printf("Failed: %d\n", fail_count);
    
    return fail_count > 0 ? 1 : 0;
}
