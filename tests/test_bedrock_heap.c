#include "../include/bedrock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int pass_count = 0;
static int fail_count = 0;

#define TEST(name) printf("  %-42s", name)
#define PASS() do { printf("PASS\n"); pass_count++; } while (0)
#define FAIL(msg) do { printf("FAIL - %s\n", msg); fail_count++; return; } while (0)
#define ASSERT(cond, msg) do { if (!(cond)) { FAIL(msg); } } while (0)

static void assert_int_root(Heap *heap, int expected, const char *msg) {
    Value *root = heap_peek(heap);
    ASSERT(root != NULL, "root should not be NULL");
    ASSERT(root->type == TYPE_INT, "root should be TYPE_INT");
    ASSERT(root->as.i == expected, msg);
}

static void insert_ints(Heap *heap, const int *vals, int n) {
    for (int i = 0; i < n; i++) {
        heap_insert(heap, vals[i]);
    }
}

void test_max_heap_insert_and_extract(void) {
    TEST("max_heap_insert_and_extract");
    int vals[] = {10, 30, 5, 42, 21};
    int expected[] = {42, 30, 21, 10, 5};
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    insert_ints(heap, vals, 5);
    for (int i = 0; i < 5; i++) {
        assert_int_root(heap, expected[i], "max extract order mismatch");
        value_free(heap_extract(heap));
    }
    ASSERT(heap_is_empty(heap), "heap should be empty after extracts");

    Heap_destroy(heap);
    PASS();
}

void test_min_heap_insert_and_extract(void) {
    TEST("min_heap_insert_and_extract");
    int vals[] = {10, 30, 5, 42, 21};
    int expected[] = {5, 10, 21, 30, 42};
    Heap *heap = Heap_new(0);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    insert_ints(heap, vals, 5);
    for (int i = 0; i < 5; i++) {
        assert_int_root(heap, expected[i], "min extract order mismatch");
        value_free(heap_extract(heap));
    }
    ASSERT(heap_is_empty(heap), "heap should be empty after extracts");

    Heap_destroy(heap);
    PASS();
}

void test_peek_does_not_remove(void) {
    TEST("peek_does_not_remove");
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    heap_insert(heap, 10);
    heap_insert(heap, 30);
    heap_insert(heap, 5);
    assert_int_root(heap, 30, "peek should return max");
    ASSERT(heap_size(heap) == 3, "peek should not remove");

    Heap_destroy(heap);
    PASS();
}

void test_extract_empty(void) {
    TEST("extract_empty");
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    Value *empty_val = heap_extract(heap);
    ASSERT(empty_val == NULL, "extract from empty heap should return NULL");
    ASSERT(heap_size(heap) == 0, "empty extract should keep size 0");

    Heap_destroy(heap);
    PASS();
}

void test_heap_property_after_each_insert(void) {
    TEST("heap_property_after_each_insert");
    int vals[] = {10, 30, 5, 42, 21};
    int roots[] = {10, 30, 30, 42, 42};
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    for (int i = 0; i < 5; i++) {
        heap_insert(heap, vals[i]);
        assert_int_root(heap, roots[i], "root should be current max");
    }

    Heap_destroy(heap);
    PASS();
}

void test_heap_property_after_each_extract(void) {
    TEST("heap_property_after_each_extract");
    int vals[] = {10, 30, 5, 42, 21};
    int roots_after[] = {30, 21, 10, 5};
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    insert_ints(heap, vals, 5);
    for (int i = 0; i < 4; i++) {
        value_free(heap_extract(heap));
        assert_int_root(heap, roots_after[i], "root should remain max after extract");
    }

    Heap_destroy(heap);
    PASS();
}

void test_heapify_max(void) {
    TEST("heapify_max");
    int expected[] = {9, 8, 5, 3, 2, 1};
    Value *arr[] = {make_int(5), make_int(3), make_int(8), make_int(1), make_int(9), make_int(2)};
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    heap_heapify(heap, arr, 6);
    for (int i = 0; i < 6; i++) {
        assert_int_root(heap, expected[i], "heapify max order mismatch");
        value_free(heap_extract(heap));
    }

    Heap_destroy(heap);
    PASS();
}

void test_heapify_min(void) {
    TEST("heapify_min");
    int expected[] = {1, 2, 3, 5, 8, 9};
    Value *arr[] = {make_int(5), make_int(3), make_int(8), make_int(1), make_int(9), make_int(2)};
    Heap *heap = Heap_new(0);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    heap_heapify(heap, arr, 6);
    for (int i = 0; i < 6; i++) {
        assert_int_root(heap, expected[i], "heapify min order mismatch");
        value_free(heap_extract(heap));
    }

    Heap_destroy(heap);
    PASS();
}

void test_heapify_vs_insert_same_result(void) {
    TEST("heapify_vs_insert_same_result");
    int vals[] = {5, 3, 8, 1, 9, 2};
    Value *arr[] = {make_int(5), make_int(3), make_int(8), make_int(1), make_int(9), make_int(2)};
    Heap *inserted = Heap_new(1);
    Heap *heapified = Heap_new(1);
    ASSERT(inserted != NULL && heapified != NULL, "Heap_new returned NULL");

    insert_ints(inserted, vals, 6);
    heap_heapify(heapified, arr, 6);

    while (!heap_is_empty(inserted)) {
        Value *a = heap_peek(inserted);
        Value *b = heap_peek(heapified);
        ASSERT(value_equals(a, b), "heapify and insert extraction sequences should match");
        value_free(heap_extract(inserted));
        value_free(heap_extract(heapified));
    }

    Heap_destroy(inserted);
    Heap_destroy(heapified);
    PASS();
}

void test_resize_trigger(void) {
    TEST("resize_trigger");
    int expected[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    for (int i = 1; i <= 9; i++) {
        heap_insert(heap, i);
    }
    ASSERT(heap->capacity >= 16, "capacity should grow past 8");

    for (int i = 0; i < 9; i++) {
        assert_int_root(heap, expected[i], "resize extraction order mismatch");
        value_free(heap_extract(heap));
    }

    Heap_destroy(heap);
    PASS();
}

void test_size_tracking(void) {
    TEST("size_tracking");
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    for (int i = 0; i < 5; i++) heap_insert(heap, i);
    value_free(heap_extract(heap));
    value_free(heap_extract(heap));
    ASSERT(heap_size(heap) == 3, "size should be 3 after two extracts");

    Heap_destroy(heap);
    PASS();
}

void test_clear(void) {
    TEST("clear");
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    for (int i = 0; i < 5; i++) heap_insert(heap, i);
    heap_clear(heap);
    ASSERT(heap_size(heap) == 0, "clear should reset size");
    ASSERT(heap_peek(heap) == NULL, "peek after clear should be NULL");
    heap_insert(heap, 42);
    assert_int_root(heap, 42, "heap should remain usable after clear");

    Heap_destroy(heap);
    PASS();
}

void test_mixed_types_max_heap(void) {
    TEST("mixed_types_max_heap");
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    heap_insert(heap, 10);
    heap_insert(heap, 9.5);
    heap_insert(heap, (char)'A');
    heap_insert(heap, 5);

    Value *root = heap_peek(heap);
    ASSERT(root != NULL && root->type == TYPE_CHAR && root->as.c == 'A',
           "TYPE_CHAR should be highest by value_compare type ordering");

    Heap_destroy(heap);
    PASS();
}

void test_string_min_heap(void) {
    TEST("string_min_heap");
    const char *expected[] = {"apple", "banana", "cherry"};
    Heap *heap = Heap_new(0);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    heap_insert(heap, "banana");
    heap_insert(heap, "apple");
    heap_insert(heap, "cherry");

    for (int i = 0; i < 3; i++) {
        Value *root = heap_peek(heap);
        ASSERT(root != NULL && root->type == TYPE_STRING, "root should be TYPE_STRING");
        ASSERT(strcmp(root->as.s, expected[i]) == 0, "string min order mismatch");
        value_free(heap_extract(heap));
    }

    Heap_destroy(heap);
    PASS();
}

void test_destroy_valgrind(void) {
    TEST("destroy_valgrind");
    Heap *heap = Heap_new(1);
    ASSERT(heap != NULL, "Heap_new returned NULL");

    heap_insert(heap, "alpha");
    heap_insert(heap, "beta");
    heap_insert(heap, 10);
    heap_insert(heap, 20);
    heap_insert(heap, 1.5);
    heap_insert(heap, 2.5);
    heap_insert(heap, (char)'x');
    heap_insert(heap, (char)'y');
    heap_insert(heap, 30);
    heap_insert(heap, "gamma");

    Heap_destroy(heap);
    PASS();
}

int main(void) {
    printf("=== CDS-Bedrock Phase 7: Heap Tests ===\n\n");

    test_max_heap_insert_and_extract();
    test_min_heap_insert_and_extract();
    test_peek_does_not_remove();
    test_extract_empty();
    test_heap_property_after_each_insert();
    test_heap_property_after_each_extract();
    test_heapify_max();
    test_heapify_min();
    test_heapify_vs_insert_same_result();
    test_resize_trigger();
    test_size_tracking();
    test_clear();
    test_mixed_types_max_heap();
    test_string_min_heap();
    test_destroy_valgrind();

    printf("\n=== Summary ===\n");
    printf("Passed : %d\n", pass_count);
    printf("Failed : %d\n", fail_count);

    return fail_count > 0 ? 1 : 0;
}
