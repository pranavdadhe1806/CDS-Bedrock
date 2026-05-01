#include "../include/hmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
 * Hash Map (Open Addressing with Linear Probing)
 * 
 * A hash map using open addressing with linear probing for collision resolution.
 * 
 * Time Complexities (average case):
 * - Put: O(1)
 * - Get: O(1)
 * - Remove: O(1)
 * 
 * Time Complexities (worst case - many collisions):
 * - Put: O(n)
 * - Get: O(n)
 * - Remove: O(n)
 */

HMap* hmap_create(size_t initial_capacity,
                  size_t (*hash_fn)(const void *key),
                  int (*key_cmp)(const void *a, const void *b),
                  void (*free_fn)(void *key, void *value)) {
    (void)initial_capacity;
    (void)hash_fn;
    (void)key_cmp;
    (void)free_fn;
    // TODO: implement
    return NULL;
}

void hmap_destroy(HMap *map) {
    (void)map;
    // TODO: implement
}

int hmap_put(HMap *map, void *key, void *value) {
    (void)map;
    (void)key;
    (void)value;
    // TODO: implement
    return 0;
}

void* hmap_get(HMap *map, const void *key) {
    (void)map;
    (void)key;
    // TODO: implement
    return NULL;
}

int hmap_remove(HMap *map, const void *key) {
    (void)map;
    (void)key;
    // TODO: implement
    return 0;
}

int hmap_contains(HMap *map, const void *key) {
    (void)map;
    (void)key;
    // TODO: implement
    return 0;
}

size_t hmap_size(HMap *map) {
    (void)map;
    // TODO: implement
    return 0;
}

int hmap_is_empty(HMap *map) {
    (void)map;
    // TODO: implement
    return 0;
}

void hmap_clear(HMap *map) {
    (void)map;
    // TODO: implement
}

int hmap_resize(HMap *map, size_t new_capacity) {
    (void)map;
    (void)new_capacity;
    // TODO: implement
    return 0;
}

double hmap_load_factor(HMap *map) {
    (void)map;
    // TODO: implement
    return 0.0;
}
