/*
 * test_edge_cases.c — Edge-case and robustness tests for CDS-Bedrock
 *
 * Covers:
 *   M-29  Out-of-bounds indices (array, list)
 *   M-30  NULL argument handling (array, list, stack, queue, heap, bst, hmap, graph)
 *   M-31  Empty container edge cases (pop empty, get empty, etc.)
 *   M-32  Graph cycles, self-loops, disconnected components
 *   M-33  Hashmap collision-heavy and resize-correctness tests
 *
 * Build:
 *   gcc -std=c11 -Wall -Wextra -Wpedantic -g \
 *       tests/test_edge_cases.c src/*.c -Iinclude -o tests/test_edge_cases
 */

#include "../include/bedrock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -----------------------------------------------------------------------
 * Minimal test framework (consistent with other test files)
 * ----------------------------------------------------------------------- */
static int pass_count = 0;
static int fail_count = 0;

#define TEST(name)  printf("  %-50s", name)
#define PASS()      do { printf("PASS\n"); pass_count++; } while(0)
#define FAIL(msg)   do { printf("FAIL  <- %s\n", msg); fail_count++; return; } while(0)

#define ASSERT(cond, msg)        do { if (!(cond)) { FAIL(msg); } } while(0)
#define ASSERT_NULL(ptr, msg)    ASSERT((ptr) == NULL,  msg)
#define ASSERT_NOTNULL(ptr, msg) ASSERT((ptr) != NULL,  msg)
#define ASSERT_EQ(a, b, msg)     ASSERT((a) == (b),     msg)

/* =======================================================================
 * M-30: NULL argument handling
 * ======================================================================= */

void test_array_null_safety(void) {
    TEST("array_null_safety");

    /* All functions should be no-ops or return safe defaults on NULL */
    BRArray_destroy(NULL);
    brarray_clear(NULL);
    brarray_print(NULL);

    ASSERT_NULL(brarray_get(NULL, 0),  "get on NULL array should return NULL");
    ASSERT_NULL(brarray_pop(NULL),     "pop on NULL array should return NULL");
    ASSERT_EQ(brarray_size(NULL), 0,   "size of NULL array should be 0");
    ASSERT_EQ(brarray_contains(NULL, NULL), 0, "contains on NULL array should be 0");

    PASS();
}

void test_list_null_safety(void) {
    TEST("list_null_safety");

    LinkedList_destroy(NULL);
    list_clear(NULL);
    list_print(NULL);
    list_reverse(NULL);

    ASSERT_NULL(list_peek_front(NULL), "peek_front on NULL should return NULL");
    ASSERT_NULL(list_peek_back(NULL),  "peek_back on NULL should return NULL");
    ASSERT_NULL(list_pop_front(NULL),  "pop_front on NULL should return NULL");
    ASSERT_NULL(list_pop_back(NULL),   "pop_back on NULL should return NULL");
    ASSERT_NULL(list_get_at(NULL, 0),  "get_at on NULL should return NULL");
    ASSERT_EQ(list_size(NULL), 0,      "size of NULL list should be 0");

    PASS();
}

void test_stack_null_safety(void) {
    TEST("stack_null_safety");

    BRStack_destroy(NULL);
    brstack_clear(NULL);
    brstack_print(NULL);

    ASSERT_NULL(brstack_pop(NULL),    "pop on NULL stack should return NULL");
    ASSERT_NULL(brstack_peek(NULL),   "peek on NULL stack should return NULL");
    ASSERT_EQ(brstack_size(NULL), 0,  "size of NULL stack should be 0");
    ASSERT_EQ(brstack_is_empty(NULL), 1, "NULL stack should be 'empty'");

    PASS();
}

void test_queue_null_safety(void) {
    TEST("queue_null_safety");

    BRQueue_destroy(NULL);
    brqueue_clear(NULL);
    brqueue_print(NULL);

    ASSERT_NULL(brqueue_dequeue(NULL), "dequeue on NULL queue should return NULL");
    ASSERT_NULL(brqueue_peek(NULL),    "peek on NULL queue should return NULL");
    ASSERT_EQ(brqueue_size(NULL), 0,   "size of NULL queue should be 0");
    ASSERT_EQ(brqueue_is_empty(NULL), 1, "NULL queue should be 'empty'");

    PASS();
}

void test_heap_null_safety(void) {
    TEST("heap_null_safety");

    Heap_destroy(NULL);
    heap_clear(NULL);
    heap_print(NULL);

    ASSERT_NULL(heap_extract(NULL), "extract on NULL heap should return NULL");
    ASSERT_NULL(heap_peek(NULL),    "peek on NULL heap should return NULL");
    ASSERT_EQ(heap_size(NULL), 0,   "size of NULL heap should be 0");
    ASSERT_EQ(heap_is_empty(NULL), 1, "NULL heap should be 'empty'");

    PASS();
}

void test_bst_null_safety(void) {
    TEST("bst_null_safety");

    BRBST_destroy(NULL);
    brbst_clear(NULL);
    brbst_inorder(NULL);
    brbst_preorder(NULL);
    brbst_postorder(NULL);

    ASSERT_NULL(brbst_find_min(NULL), "find_min on NULL tree should return NULL");
    ASSERT_NULL(brbst_find_max(NULL), "find_max on NULL tree should return NULL");
    ASSERT_EQ(brbst_height(NULL), 0,  "height of NULL tree should be 0");
    ASSERT_EQ(brbst_size(NULL), 0,    "size of NULL tree should be 0");
    ASSERT_EQ(brbst_is_empty(NULL), 1,"NULL tree should be 'empty'");

    PASS();
}

void test_graph_null_safety(void) {
    TEST("graph_null_safety");

    graph_destroy(NULL);
    graph_print(NULL);
    graph_bfs(NULL, 0, NULL);
    graph_dfs(NULL, 0, NULL);

    ASSERT_EQ(graph_add_edge(NULL, 0, 1, NULL), 0,  "add_edge on NULL graph should fail");
    ASSERT_EQ(graph_remove_edge(NULL, 0, 1), 0,     "remove_edge on NULL graph should fail");
    ASSERT_EQ(graph_has_edge(NULL, 0, 1), 0,         "has_edge on NULL graph should return 0");
    ASSERT_NULL(graph_get_adjacent(NULL, 0),          "get_adjacent on NULL graph should return NULL");
    ASSERT_EQ(graph_num_vertices(NULL), 0,            "num_vertices on NULL graph should be 0");
    ASSERT_EQ(graph_num_edges(NULL), 0,               "num_edges on NULL graph should be 0");

    PASS();
}

/* =======================================================================
 * M-29: Out-of-bounds index handling
 * ======================================================================= */

void test_array_oob_access(void) {
    TEST("array_oob_access");
    BRArray *arr = BRArray_new();
    ASSERT_NOTNULL(arr, "BRArray_new returned NULL");

    push(arr, 10);
    push(arr, 20);

    /* Out-of-bounds get */
    ASSERT_NULL(brarray_get(arr, -1),  "get(-1) should return NULL");
    ASSERT_NULL(brarray_get(arr, 2),   "get(2) on size-2 array should return NULL");
    ASSERT_NULL(brarray_get(arr, 100), "get(100) should return NULL");

    /* Out-of-bounds delete — should be no-op */
    brarray_delete(arr, -1);
    brarray_delete(arr, 99);
    ASSERT_EQ(brarray_size(arr), 2, "size should remain 2 after OOB deletes");

    /* Out-of-bounds update — should be no-op */
    Value *v = make_int(999);
    brarray_update(arr, -1, v);
    /* v should have been freed by update's error path or we need to free it.
     * The current API frees the Value on out-of-bounds, so size stays 2. */
    ASSERT_EQ(brarray_size(arr), 2, "size should remain 2 after OOB update");

    BRArray_destroy(arr);
    PASS();
}

void test_list_oob_access(void) {
    TEST("list_oob_access");
    LinkedList *list = LinkedList_new();
    ASSERT_NOTNULL(list, "LinkedList_new returned NULL");

    push_back(list, 10);
    push_back(list, 20);

    /* Out-of-bounds get */
    ASSERT_NULL(list_get_at(list, -1),  "get_at(-1) should return NULL");
    ASSERT_NULL(list_get_at(list, 2),   "get_at(2) on size-2 list should return NULL");
    ASSERT_NULL(list_get_at(list, 100), "get_at(100) should return NULL");

    /* Out-of-bounds remove — should be no-op */
    list_remove_at(list, -1);
    list_remove_at(list, 99);
    ASSERT_EQ(list_size(list), 2, "size should remain 2 after OOB removes");

    LinkedList_destroy(list);
    PASS();
}

/* =======================================================================
 * M-31: Empty container edge cases
 * ======================================================================= */

void test_pop_empty_array(void) {
    TEST("pop_empty_array");
    BRArray *arr = BRArray_new();
    ASSERT_NOTNULL(arr, "BRArray_new returned NULL");

    ASSERT_NULL(brarray_pop(arr), "pop from empty array should return NULL");
    ASSERT_EQ(brarray_size(arr), 0, "size should remain 0");

    BRArray_destroy(arr);
    PASS();
}

void test_pop_empty_stack(void) {
    TEST("pop_empty_stack");
    BRStack *s = BRStack_new();
    ASSERT_NOTNULL(s, "BRStack_new returned NULL");

    ASSERT_NULL(brstack_pop(s),  "pop from empty stack should return NULL");
    ASSERT_NULL(brstack_peek(s), "peek on empty stack should return NULL");
    ASSERT_EQ(brstack_size(s), 0, "size should remain 0");

    BRStack_destroy(s);
    PASS();
}

void test_dequeue_empty_queue(void) {
    TEST("dequeue_empty_queue");
    BRQueue *q = BRQueue_new();
    ASSERT_NOTNULL(q, "BRQueue_new returned NULL");

    ASSERT_NULL(brqueue_dequeue(q), "dequeue from empty queue should return NULL");
    ASSERT_NULL(brqueue_peek(q),    "peek on empty queue should return NULL");
    ASSERT_EQ(brqueue_size(q), 0,   "size should remain 0");

    BRQueue_destroy(q);
    PASS();
}

void test_extract_empty_heap(void) {
    TEST("extract_empty_heap");
    Heap *h = Heap_new(1);
    ASSERT_NOTNULL(h, "Heap_new returned NULL");

    ASSERT_NULL(heap_extract(h), "extract from empty heap should return NULL");
    ASSERT_NULL(heap_peek(h),    "peek on empty heap should return NULL");
    ASSERT_EQ(heap_size(h), 0,   "size should remain 0");

    Heap_destroy(h);
    PASS();
}

void test_pop_empty_list(void) {
    TEST("pop_empty_list");
    LinkedList *list = LinkedList_new();
    ASSERT_NOTNULL(list, "LinkedList_new returned NULL");

    ASSERT_NULL(list_pop_front(list), "pop_front from empty list should return NULL");
    ASSERT_NULL(list_pop_back(list),  "pop_back from empty list should return NULL");
    ASSERT_NULL(list_peek_front(list),"peek_front on empty list should return NULL");
    ASSERT_NULL(list_peek_back(list), "peek_back on empty list should return NULL");
    ASSERT_EQ(list_size(list), 0,     "size should remain 0");

    LinkedList_destroy(list);
    PASS();
}

void test_bst_empty_operations(void) {
    TEST("bst_empty_operations");
    BRBST *tree = BRBST_new();
    ASSERT_NOTNULL(tree, "BRBST_new returned NULL");

    ASSERT_NULL(brbst_find_min(tree), "find_min on empty tree should return NULL");
    ASSERT_NULL(brbst_find_max(tree), "find_max on empty tree should return NULL");
    ASSERT_EQ(brbst_height(tree), 0,  "height of empty tree should be 0");
    ASSERT_EQ(brbst_size(tree), 0,    "size of empty tree should be 0");
    ASSERT_EQ(brbst_is_empty(tree), 1,"empty tree should be empty");

    /* Remove from empty tree — should be no-op */
    Value tmp = {TYPE_INT, {.i = 42}};
    brbst_remove(tree, &tmp);
    ASSERT_EQ(brbst_size(tree), 0, "size should remain 0 after remove from empty");

    BRBST_destroy(tree);
    PASS();
}

void test_clear_then_reuse(void) {
    TEST("clear_then_reuse");

    /* Array */
    BRArray *arr = BRArray_new();
    push(arr, 1);
    push(arr, 2);
    brarray_clear(arr);
    ASSERT_EQ(brarray_size(arr), 0, "array size should be 0 after clear");
    push(arr, 3);
    ASSERT_EQ(brarray_size(arr), 1, "array should accept items after clear");
    BRArray_destroy(arr);

    /* Stack */
    BRStack *s = BRStack_new();
    stack_push(s, 1);
    brstack_clear(s);
    ASSERT_EQ(brstack_size(s), 0, "stack size should be 0 after clear");
    stack_push(s, 2);
    ASSERT_EQ(brstack_size(s), 1, "stack should accept items after clear");
    BRStack_destroy(s);

    /* Queue */
    BRQueue *q = BRQueue_new();
    enqueue(q, 1);
    brqueue_clear(q);
    ASSERT_EQ(brqueue_size(q), 0, "queue size should be 0 after clear");
    enqueue(q, 2);
    ASSERT_EQ(brqueue_size(q), 1, "queue should accept items after clear");
    BRQueue_destroy(q);

    /* Heap */
    Heap *h = Heap_new(1);
    heap_insert(h, 1);
    heap_clear(h);
    ASSERT_EQ(heap_size(h), 0, "heap size should be 0 after clear");
    heap_insert(h, 2);
    ASSERT_EQ(heap_size(h), 1, "heap should accept items after clear");
    Heap_destroy(h);

    /* BST */
    BRBST *t = BRBST_new();
    bst_insert(t, 1);
    brbst_clear(t);
    ASSERT_EQ(brbst_size(t), 0, "bst size should be 0 after clear");
    bst_insert(t, 2);
    ASSERT_EQ(brbst_size(t), 1, "bst should accept items after clear");
    BRBST_destroy(t);

    PASS();
}

/* =======================================================================
 * M-32: Graph edge cases — self-loops
 * ======================================================================= */

void test_graph_self_loop(void) {
    TEST("graph_self_loop");
    Graph *g = graph_new(3, 1);
    ASSERT_NOTNULL(g, "graph_new returned NULL");

    /* Self-loop: edge from vertex 0 to itself */
    ASSERT_EQ(graph_edge(g, 0, 0), 1, "self-loop should be allowed");
    ASSERT_EQ(graph_connected(g, 0, 0), 1, "self-loop should be detectable");
    ASSERT_EQ(graph_edges(g), 1, "self-loop should count as 1 edge");

    /* BFS from self-loop vertex should not infinite loop */
    graph_bfs(g, 0, NULL);
    graph_dfs(g, 0, NULL);

    graph_destroy(g);
    PASS();
}

void test_graph_single_vertex(void) {
    TEST("graph_single_vertex");
    Graph *g = graph_new(1, 1);
    ASSERT_NOTNULL(g, "graph_new returned NULL");

    ASSERT_EQ(graph_vertices(g), 1, "should have 1 vertex");
    ASSERT_EQ(graph_edges(g), 0,    "should have 0 edges");

    /* BFS/DFS on single vertex should work */
    graph_bfs(g, 0, NULL);
    graph_dfs(g, 0, NULL);

    graph_destroy(g);
    PASS();
}

/* =======================================================================
 * M-33: Hashmap collision-heavy and resize correctness
 * ======================================================================= */

void test_hmap_many_inserts_and_deletes(void) {
    TEST("hmap_many_inserts_and_deletes");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    /* Insert 50 entries — forces multiple resizes */
    for (int i = 0; i < 50; i++) {
        hmap_put(m, i, i * 100);
    }
    ASSERT_EQ(hmap_size(m), 50, "size should be 50 after 50 inserts");

    /* Verify all 50 are accessible */
    for (int i = 0; i < 50; i++) {
        Value *v = hmap_lookup(m, i);
        ASSERT_NOTNULL(v, "key should survive multiple resizes");
        ASSERT_EQ(v->as.i, i * 100, "value should match after resize");
    }

    /* Delete every other entry */
    for (int i = 0; i < 50; i += 2) {
        ASSERT_EQ(hmap_delete(m, i), 1, "delete should succeed for existing key");
    }
    ASSERT_EQ(hmap_size(m), 25, "size should be 25 after 25 deletes");

    /* Remaining entries should still be accessible */
    for (int i = 1; i < 50; i += 2) {
        Value *v = hmap_lookup(m, i);
        ASSERT_NOTNULL(v, "surviving key should still be accessible");
        ASSERT_EQ(v->as.i, i * 100, "surviving value should match");
    }

    /* Deleted entries should be gone */
    for (int i = 0; i < 50; i += 2) {
        Value *v = hmap_lookup(m, i);
        ASSERT_NULL(v, "deleted key should return NULL");
    }

    HMap_destroy(m);
    PASS();
}

void test_hmap_insert_delete_cycle(void) {
    TEST("hmap_insert_delete_cycle_tombstones");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    /* Insert and delete the same keys many times — tests tombstone handling */
    for (int round = 0; round < 20; round++) {
        for (int i = 0; i < 10; i++) {
            hmap_put(m, i, round * 10 + i);
        }
        ASSERT_EQ(hmap_size(m), 10, "size should be 10 after insert round");

        for (int i = 0; i < 10; i++) {
            hmap_delete(m, i);
        }
        ASSERT_EQ(hmap_size(m), 0, "size should be 0 after delete round");
    }

    /* Map should still be usable after heavy tombstone creation */
    hmap_put(m, 999, 42);
    Value *v = hmap_lookup(m, 999);
    ASSERT_NOTNULL(v, "should find key after tombstone cycle");
    ASSERT_EQ(v->as.i, 42, "value should be correct after tombstone cycle");

    HMap_destroy(m);
    PASS();
}

void test_hmap_string_key_collision(void) {
    TEST("hmap_string_key_collision");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    /* Insert many string keys — likely to collide in a small table */
    const char *keys[] = {
        "alpha", "bravo", "charlie", "delta", "echo",
        "foxtrot", "golf", "hotel", "india", "juliet",
        "kilo", "lima", "mike", "november", "oscar",
        "papa", "quebec", "romeo", "sierra", "tango"
    };
    int n = 20;

    for (int i = 0; i < n; i++) {
        hmap_put(m, keys[i], i);
    }
    ASSERT_EQ(hmap_size(m), n, "all string keys should be inserted");

    /* Verify all are accessible */
    for (int i = 0; i < n; i++) {
        Value *v = hmap_lookup(m, keys[i]);
        ASSERT_NOTNULL(v, "string key should be found");
        ASSERT_EQ(v->as.i, i, "string key value should match");
    }

    HMap_destroy(m);
    PASS();
}

/* =======================================================================
 * Large-scale stress tests
 * ======================================================================= */

void test_array_large_scale(void) {
    TEST("array_large_scale_1000");
    BRArray *arr = BRArray_new();
    ASSERT_NOTNULL(arr, "BRArray_new returned NULL");

    /* Push 1000 elements */
    for (int i = 0; i < 1000; i++) {
        push(arr, i);
    }
    ASSERT_EQ(brarray_size(arr), 1000, "size should be 1000");

    /* Verify random access */
    Value *v500 = brarray_get(arr, 500);
    ASSERT_NOTNULL(v500, "get(500) should succeed");
    ASSERT_EQ(v500->as.i, 500, "value at index 500 should be 500");

    /* Pop all */
    for (int i = 999; i >= 0; i--) {
        Value *v = brarray_pop(arr);
        ASSERT_NOTNULL(v, "pop should return a value");
        ASSERT_EQ(v->as.i, i, "pop should return in LIFO order");
        value_free(v);
    }
    ASSERT_EQ(brarray_size(arr), 0, "size should be 0 after popping all");

    BRArray_destroy(arr);
    PASS();
}

void test_bst_sorted_insert_destroy(void) {
    TEST("bst_sorted_insert_1000_destroy");
    BRBST *tree = BRBST_new();
    ASSERT_NOTNULL(tree, "BRBST_new returned NULL");

    /* Sorted insert creates worst-case O(n) depth */
    for (int i = 0; i < 1000; i++) {
        bst_insert(tree, i);
    }
    ASSERT_EQ(brbst_size(tree), 1000, "size should be 1000");

    /* Verify min and max */
    Value *vmin = brbst_find_min(tree);
    ASSERT_NOTNULL(vmin, "find_min should succeed");
    ASSERT_EQ(vmin->as.i, 0, "min should be 0");

    Value *vmax = brbst_find_max(tree);
    ASSERT_NOTNULL(vmax, "find_max should succeed");
    ASSERT_EQ(vmax->as.i, 999, "max should be 999");

    /* This tests the iterative destroy — should not stack overflow */
    BRBST_destroy(tree);
    PASS();
}

/* =======================================================================
 * main
 * ======================================================================= */
int main(void) {
    printf("=== CDS-Bedrock Edge Case Tests ===\n\n");

    printf("--- NULL safety ---\n");
    test_array_null_safety();
    test_list_null_safety();
    test_stack_null_safety();
    test_queue_null_safety();
    test_heap_null_safety();
    test_bst_null_safety();
    test_graph_null_safety();

    printf("\n--- Out-of-bounds ---\n");
    test_array_oob_access();
    test_list_oob_access();

    printf("\n--- Empty container operations ---\n");
    test_pop_empty_array();
    test_pop_empty_stack();
    test_dequeue_empty_queue();
    test_extract_empty_heap();
    test_pop_empty_list();
    test_bst_empty_operations();
    test_clear_then_reuse();

    printf("\n--- Graph edge cases ---\n");
    test_graph_self_loop();
    test_graph_single_vertex();

    printf("\n--- Hashmap stress ---\n");
    test_hmap_many_inserts_and_deletes();
    test_hmap_insert_delete_cycle();
    test_hmap_string_key_collision();

    printf("\n--- Large-scale ---\n");
    test_array_large_scale();
    test_bst_sorted_insert_destroy();

    printf("\n=== Summary ===\n");
    printf("Passed : %d\n", pass_count);
    printf("Failed : %d\n", fail_count);

    return fail_count > 0 ? 1 : 0;
}
