/*
 * test_bedrock_hmap.c - comprehensive test suite for CDS-Bedrock Phase 6: HMap
 *
 * Tests use the high-level macros from bedrock.h (hmap_put, hmap_lookup,
 * hmap_delete) as well as the lower-level public API directly.
 *
 * Build:
 *   gcc -std=c11 -Wall -Wextra -g \
 *       tests/test_bedrock_hmap.c src/bedrock_hmap.c src/value.c -I include -o tests/test_bedrock_hmap
 *
 * Valgrind:
 *   valgrind --leak-check=full ./tests/test_bedrock_hmap
 */

#include "../include/bedrock.h"   /* pulls in bedrock_hmap.h, value.h, and all macros */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -----------------------------------------------------------------------
 * Minimal test framework
 * ----------------------------------------------------------------------- */
static int pass_count = 0;
static int fail_count = 0;

#define TEST(name) printf("  %-45s", name)

#define PASS() do { printf("PASS\n"); pass_count++; } while(0)

#define FAIL(msg) do {                                          \
    printf("FAIL  ← %s\n", msg);                               \
    fail_count++;                                               \
    return;                                                     \
} while(0)

#define ASSERT(cond, msg) do {                                  \
    if (!(cond)) { FAIL(msg); }                                 \
} while(0)

#define ASSERT_NULL(ptr, msg)    ASSERT((ptr) == NULL,  msg)
#define ASSERT_NOTNULL(ptr, msg) ASSERT((ptr) != NULL,  msg)
#define ASSERT_EQ(a, b, msg)     ASSERT((a) == (b),     msg)

/* -----------------------------------------------------------------------
 * Test: put an int-keyed entry and look it up
 * ----------------------------------------------------------------------- */
void test_put_and_get_int_key(void) {
    TEST("put_and_get_int_key");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, 42, 100);
    Value *v = hmap_lookup(m, 42);
    ASSERT_NOTNULL(v, "lookup returned NULL");
    ASSERT_EQ(v->type, TYPE_INT, "expected TYPE_INT value");
    ASSERT_EQ(v->as.i, 100, "expected value 100");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: put a string-keyed entry and look it up
 * ----------------------------------------------------------------------- */
void test_put_and_get_string_key(void) {
    TEST("put_and_get_string_key");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, "name", "Pranav");
    Value *v = hmap_lookup(m, "name");
    ASSERT_NOTNULL(v, "lookup returned NULL");
    ASSERT_EQ(v->type, TYPE_STRING, "expected TYPE_STRING value");
    ASSERT_EQ(strcmp(v->as.s, "Pranav"), 0, "expected value 'Pranav'");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: inserting the same key twice → update, size stays 1
 * ----------------------------------------------------------------------- */
void test_put_duplicate_key(void) {
    TEST("put_duplicate_key");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, "x", 1);
    hmap_put(m, "x", 2);

    ASSERT_EQ(hmap_size(m), 1, "size should stay 1 after duplicate put");

    Value *v = hmap_lookup(m, "x");
    ASSERT_NOTNULL(v, "lookup returned NULL");
    ASSERT_EQ(v->as.i, 2, "value should be updated to 2");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: looking up a key that was never inserted → NULL
 * ----------------------------------------------------------------------- */
void test_get_missing_key(void) {
    TEST("get_missing_key");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, "present", 1);
    Value *v = hmap_lookup(m, "absent");
    ASSERT_NULL(v, "lookup of absent key should return NULL");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: remove an existing entry → lookup returns NULL, size decrements
 * ----------------------------------------------------------------------- */
void test_remove_existing(void) {
    TEST("remove_existing");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, "age", 21);
    ASSERT_EQ(hmap_size(m), 1, "size should be 1 before delete");

    int removed = hmap_delete(m, "age");
    ASSERT_EQ(removed, 1, "hmap_delete should return 1 for found key");
    ASSERT_EQ(hmap_size(m), 0, "size should be 0 after delete");

    Value *v = hmap_lookup(m, "age");
    ASSERT_NULL(v, "lookup after delete should return NULL");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: remove a key that was never inserted → returns 0, no crash
 * ----------------------------------------------------------------------- */
void test_remove_missing(void) {
    TEST("remove_missing");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    int removed = hmap_delete(m, "ghost");
    ASSERT_EQ(removed, 0, "hmap_delete of missing key should return 0");
    ASSERT_EQ(hmap_size(m), 0, "size should stay 0");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: tombstone probe chain — delete A, then still find B
 *
 * Strategy: insert enough entries that two of them collide on the same
 * initial slot (or at least land in consecutive slots), delete the first,
 * then verify the second is still reachable.
 *
 * We use integer keys 0 and 16 — both hash to slot 0 in a capacity-16 map
 * via _hash_int: (0 * 2654435761) % 16 = 0, (16 * 2654435761) % 16 = 0.
 * ----------------------------------------------------------------------- */
void test_tombstone_probe_chain(void) {
    TEST("tombstone_probe_chain");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    /* Both hash to slot 0 in a capacity-16 map */
    hmap_put(m, 0,  100);   /* A — lands at slot 0      */
    hmap_put(m, 16, 200);   /* B — collides, probes to slot 1 */

    /* Remove A → slot 0 becomes SLOT_TOMBSTONE */
    int removed = hmap_delete(m, 0);
    ASSERT_EQ(removed, 1, "should remove key 0 (A)");

    /* B must still be findable even though slot 0 is a tombstone */
    Value *v = hmap_lookup(m, 16);
    ASSERT_NOTNULL(v, "key 16 (B) should still be accessible after A is deleted");
    ASSERT_EQ(v->as.i, 200, "value for key 16 should be 200");

    /* A must be gone */
    Value *va = hmap_lookup(m, 0);
    ASSERT_NULL(va, "key 0 (A) should return NULL after removal");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: insertion reuses the first tombstone in a probe chain.
 * ----------------------------------------------------------------------- */
void test_insert_reuses_tombstone(void) {
    TEST("insert_reuses_tombstone");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, 0, 100);
    hmap_put(m, 16, 200);
    hmap_put(m, 32, 300);

    ASSERT_EQ(hmap_delete(m, 16), 1, "should remove middle colliding key");
    ASSERT_EQ(m->entries[1].state, SLOT_TOMBSTONE, "slot 1 should be a tombstone");

    hmap_put(m, 48, 400);

    ASSERT_EQ(m->entries[1].state, SLOT_OCCUPIED, "insert should reuse first tombstone");
    ASSERT_NOTNULL(m->entries[1].key, "reused tombstone should have a key");
    ASSERT_EQ(m->entries[1].key->as.i, 48, "key 48 should occupy the tombstone slot");

    Value *v = hmap_lookup(m, 32);
    ASSERT_NOTNULL(v, "later probe-chain key should remain accessible");
    ASSERT_EQ(v->as.i, 300, "key 32 value should remain 300");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: insert 12 entries (past 0.7 × 16 = 11.2) → resize fires,
 *       all entries still accessible
 * ----------------------------------------------------------------------- */
void test_resize_trigger(void) {
    TEST("resize_trigger");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    /* Insert 12 entries — 12th insert crosses the 0.7 threshold */
    for (int i = 0; i < 12; i++) {
        hmap_put(m, i, i * 10);
    }

    ASSERT_EQ(hmap_size(m), 12, "size should be 12 after 12 inserts");

    /* Verify every key survives the resize */
    for (int i = 0; i < 12; i++) {
        Value *v = hmap_lookup(m, i);
        ASSERT_NOTNULL(v, "key should survive resize");
        ASSERT_EQ(v->as.i, i * 10, "value should survive resize");
    }

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: hmap_contains — true case
 * ----------------------------------------------------------------------- */
void test_contains_true(void) {
    TEST("contains_true");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, "key", 99);

    Value tmp = {TYPE_STRING, {.s = (char *)"key"}};
    ASSERT_EQ(hmap_contains(m, &tmp), 1, "contains should return 1 for existing key");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: hmap_contains — false case
 * ----------------------------------------------------------------------- */
void test_contains_false(void) {
    TEST("contains_false");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    Value tmp = {TYPE_STRING, {.s = (char *)"nope"}};
    ASSERT_EQ(hmap_contains(m, &tmp), 0, "contains should return 0 for missing key");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: hmap_clear — size becomes 0, all lookups return NULL
 * ----------------------------------------------------------------------- */
void test_clear(void) {
    TEST("clear");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, "a", 1);
    hmap_put(m, "b", 2);
    hmap_put(m, "c", 3);
    hmap_put(m, "d", 4);
    hmap_put(m, "e", 5);

    ASSERT_EQ(hmap_size(m), 5, "size should be 5 before clear");

    hmap_clear(m);

    ASSERT_EQ(hmap_size(m), 0, "size should be 0 after clear");

    Value *v = hmap_lookup(m, "a");
    ASSERT_NULL(v, "lookup after clear should return NULL");

    /* Map should still be usable after clear */
    hmap_put(m, "fresh", 42);
    ASSERT_EQ(hmap_size(m), 1, "map should accept new entries after clear");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: hmap_is_empty
 * ----------------------------------------------------------------------- */
void test_is_empty(void) {
    TEST("is_empty");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    ASSERT_EQ(hmap_is_empty(m), 1, "new map should be empty");

    hmap_put(m, "x", 1);
    ASSERT_EQ(hmap_is_empty(m), 0, "map with entry should not be empty");

    hmap_clear(m);
    ASSERT_EQ(hmap_is_empty(m), 1, "map should be empty after clear");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: mix int and string keys in the same map
 * ----------------------------------------------------------------------- */
void test_mixed_key_types(void) {
    TEST("mixed_key_types");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, 1,      "one");
    hmap_put(m, "two",  2);
    hmap_put(m, (char)'Z', "zeta");

    Value *v1 = hmap_lookup(m, 1);
    ASSERT_NOTNULL(v1, "int key 1 should be found");
    ASSERT_EQ(strcmp(v1->as.s, "one"), 0, "value for key 1 should be 'one'");

    Value *v2 = hmap_lookup(m, "two");
    ASSERT_NOTNULL(v2, "string key 'two' should be found");
    ASSERT_EQ(v2->as.i, 2, "value for key 'two' should be 2");

    Value *v3 = hmap_lookup(m, (char)'Z');
    ASSERT_NOTNULL(v3, "char key 'Z' should be found");
    ASSERT_EQ(strcmp(v3->as.s, "zeta"), 0, "value for key 'Z' should be 'zeta'");

    ASSERT_EQ(hmap_size(m), 3, "size should be 3 with mixed key types");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: same key type, different value types
 * ----------------------------------------------------------------------- */
void test_mixed_value_types(void) {
    TEST("mixed_value_types");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, "int_val",    42);
    hmap_put(m, "double_val", 3.14);
    hmap_put(m, "char_val",   (char)'X');
    hmap_put(m, "str_val",    "hello");

    Value *vi = hmap_lookup(m, "int_val");
    ASSERT_NOTNULL(vi, "int value should be found");
    ASSERT_EQ(vi->type, TYPE_INT, "should be TYPE_INT");
    ASSERT_EQ(vi->as.i, 42, "int value should be 42");

    Value *vd = hmap_lookup(m, "double_val");
    ASSERT_NOTNULL(vd, "double value should be found");
    ASSERT_EQ(vd->type, TYPE_DOUBLE, "should be TYPE_DOUBLE");

    Value *vc = hmap_lookup(m, "char_val");
    ASSERT_NOTNULL(vc, "char value should be found");
    ASSERT_EQ(vc->type, TYPE_CHAR, "should be TYPE_CHAR");
    ASSERT_EQ(vc->as.c, 'X', "char value should be 'X'");

    Value *vs = hmap_lookup(m, "str_val");
    ASSERT_NOTNULL(vs, "string value should be found");
    ASSERT_EQ(vs->type, TYPE_STRING, "should be TYPE_STRING");
    ASSERT_EQ(strcmp(vs->as.s, "hello"), 0, "string value should be 'hello'");

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: create, put 10 entries, destroy — run under Valgrind for leaks
 * ----------------------------------------------------------------------- */
void test_destroy_valgrind(void) {
    TEST("destroy_valgrind (no leaks expected)");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, "alpha",   1);
    hmap_put(m, "beta",    2.71828);
    hmap_put(m, "gamma",   (char)'G');
    hmap_put(m, "delta",   "four");
    hmap_put(m, 100,       "hundred");
    hmap_put(m, 200,       200);
    hmap_put(m, 3.14,      "pi");
    hmap_put(m, (char)'A', 65);
    hmap_put(m, "dup_key", 999);
    hmap_put(m, "dup_key", 1000); /* duplicate — previous value must be freed */

    ASSERT_EQ(hmap_size(m), 9, "size should be 9 (one duplicate replaced)");

    /* Partial removal before destroy to exercise tombstone + free paths */
    hmap_delete(m, "beta");
    hmap_delete(m, 100);

    HMap_destroy(m);  /* must free everything — Valgrind will verify */
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: hmap_print smoke test (just checks it doesn't crash)
 * ----------------------------------------------------------------------- */
void test_print_smoke(void) {
    TEST("print_smoke");
    HMap *m = HMap_new();
    ASSERT_NOTNULL(m, "HMap_new returned NULL");

    hmap_put(m, "name",  "Pranav");
    hmap_put(m, "age",   21);
    hmap_put(m, "grade", (char)'A');

    printf("\n    ");  /* indent for readability inside test runner */
    hmap_print(m);

    HMap_destroy(m);
    PASS();
}

/* -----------------------------------------------------------------------
 * Test: NULL map — all functions must handle gracefully (no crash)
 * ----------------------------------------------------------------------- */
void test_null_map_safety(void) {
    TEST("null_map_safety");

    /* None of these should crash */
    HMap_destroy(NULL);
    hmap_clear(NULL);
    hmap_print(NULL);

    Value tmp = {TYPE_INT, {.i = 0}};
    ASSERT_NULL(hmap_get(NULL, &tmp),     "get on NULL map should return NULL");
    ASSERT_EQ  (hmap_remove(NULL, &tmp),  0, "remove on NULL map should return 0");
    ASSERT_EQ  (hmap_contains(NULL, &tmp),0, "contains on NULL map should return 0");
    ASSERT_EQ  (hmap_size(NULL),          0, "size of NULL map should be 0");
    ASSERT_EQ  (hmap_is_empty(NULL),      1, "NULL map should be 'empty'");

    PASS();
}

/* -----------------------------------------------------------------------
 * main
 * ----------------------------------------------------------------------- */
int main(void) {
    printf("=== CDS-Bedrock Phase 6: Hash Map Tests ===\n\n");

    test_put_and_get_int_key();
    test_put_and_get_string_key();
    test_put_duplicate_key();
    test_get_missing_key();
    test_remove_existing();
    test_remove_missing();
    test_tombstone_probe_chain();
    test_insert_reuses_tombstone();
    test_resize_trigger();
    test_contains_true();
    test_contains_false();
    test_clear();
    test_is_empty();
    test_mixed_key_types();
    test_mixed_value_types();
    test_destroy_valgrind();
    test_print_smoke();
    test_null_map_safety();

    printf("\n=== Summary ===\n");
    printf("Passed : %d\n", pass_count);
    printf("Failed : %d\n", fail_count);

    return fail_count > 0 ? 1 : 0;
}
