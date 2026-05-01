#ifndef HMAP_H
#define HMAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void  *key;
    void  *value;
    int    occupied;      // 1 = slot in use, 0 = empty, -1 = deleted (tombstone)
} HMapEntry;

typedef struct {
    HMapEntry *buckets;
    size_t     capacity;
    size_t     size;
    size_t   (*hash_fn)(const void *key);
    int      (*key_cmp)(const void *a, const void *b);
    void     (*free_fn)(void *key, void *value);
} HMap;

// Create a new hash map with initial capacity
HMap* hmap_create(size_t initial_capacity,
                  size_t (*hash_fn)(const void *key),
                  int (*key_cmp)(const void *a, const void *b),
                  void (*free_fn)(void *key, void *value));

// Destroy the hash map and free all memory
void hmap_destroy(HMap *map);

// Insert or update a key-value pair
int hmap_put(HMap *map, void *key, void *value);

// Get the value associated with a key
void* hmap_get(HMap *map, const void *key);

// Remove a key-value pair from the map
int hmap_remove(HMap *map, const void *key);

// Check if a key exists in the map
int hmap_contains(HMap *map, const void *key);

// Get the number of key-value pairs in the map
size_t hmap_size(HMap *map);

// Check if the map is empty
int hmap_is_empty(HMap *map);

// Remove all entries (calls free_fn if provided)
void hmap_clear(HMap *map);

// Resize the hash map to new capacity
int hmap_resize(HMap *map, size_t new_capacity);

// Get the current load factor
double hmap_load_factor(HMap *map);

#endif // HMAP_H
