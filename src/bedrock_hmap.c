/*
 * bedrock_hmap.c - Hash Map implementation for CDS-Bedrock
 *
 * Strategy : open addressing with linear probing
 * Load threshold : 0.7  →  resize to 2× capacity
 * Deletion : tombstone slots (SLOT_TOMBSTONE) allow probe chains
 *            to remain intact; resize discards all tombstones.
 *
 * Time complexities (average):
 *   put / get / remove / contains : O(1)
 * Time complexities (worst — degenerate hash):
 *   all of the above : O(n)
 */

#include "../include/bedrock_hmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* ===================================================================
 * Internal hash functions — one per primitive type
 * =================================================================== */

static unsigned int _hash_int(int val, int capacity) {
    return (unsigned int)((unsigned int)val * 2654435761u) % (unsigned int)capacity;
}

static unsigned int _hash_double(double val, int capacity) {
    unsigned long long bits;
    memcpy(&bits, &val, sizeof(bits));
    return (unsigned int)(bits * 2654435761u) % (unsigned int)capacity;
}

static unsigned int _hash_char(char val, int capacity) {
    return (unsigned int)((unsigned char)val * 31u) % (unsigned int)capacity;
}

static unsigned int _hash_string(const char *val, int capacity) {
    if (val == NULL) return 0;
    /* djb2 */
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*val++)) {
        hash = ((hash << 5) + hash) + (unsigned long)c;
    }
    return (unsigned int)(hash % (unsigned long)capacity);
}

/* Dispatch hash function for any Value */
static unsigned int _hash_value(const Value *val, int capacity) {
    if (val == NULL || capacity <= 0) return 0;
    switch (val->type) {
        case TYPE_INT:    return _hash_int(val->as.i, capacity);
        case TYPE_DOUBLE: return _hash_double(val->as.d, capacity);
        case TYPE_CHAR:   return _hash_char(val->as.c, capacity);
        case TYPE_STRING: return _hash_string(val->as.s, capacity);
    }
    return 0;
}

/* ===================================================================
 * Internal helpers
 * =================================================================== */

/* Forward declaration — _hmap_put_value calls _hmap_resize */
static void _hmap_resize(HMap *map);

/*
 * _hmap_put_value — insert or update a heap-allocated key/value pair.
 *
 * Ownership: on success the map owns *key and *value.
 * Caller must NOT free them after calling this.
 */
static void _hmap_put_value(HMap *map, Value *key, Value *value) {
    if (map == NULL || key == NULL || value == NULL) {
        value_free(key);
        value_free(value);
        return;
    }

    /* Resize before inserting if load (live + tombstones) ≥ 0.7.
     * Tombstones degrade probe chains just like live entries, so we must
     * count them to avoid O(n) lookups after many insert/delete cycles. */
    int occupied = 0;
    for (int j = 0; j < map->capacity; j++) {
        if (map->entries[j].state != SLOT_EMPTY) occupied++;
    }
    if ((float)occupied / (float)map->capacity >= 0.7f) {
        _hmap_resize(map);
    }

    unsigned int idx = _hash_value(key, map->capacity);
    int first_tombstone = -1;  /* index of first SLOT_TOMBSTONE seen */

    for (int i = 0; i < map->capacity; i++) {
        int probe = (int)((idx + (unsigned int)i) % (unsigned int)map->capacity);
        HMapEntry *entry = &map->entries[probe];

        if (entry->state == SLOT_EMPTY) {
            /* Key definitely not in map — insert here or at tombstone */
            int insert_at = (first_tombstone != -1) ? first_tombstone : probe;
            map->entries[insert_at].key   = key;
            map->entries[insert_at].value = value;
            map->entries[insert_at].state = SLOT_OCCUPIED;
            map->size++;
            return;
        }

        if (entry->state == SLOT_TOMBSTONE) {
            if (first_tombstone == -1) {
                first_tombstone = probe;
            }
            /* Keep probing — the key may still exist further along */
            continue;
        }

        /* SLOT_OCCUPIED — check for key match */
        if (value_equals(entry->key, key)) {
            /* Duplicate key: free old key+value, store new ones */
            value_free(entry->key);
            value_free(entry->value);
            entry->key   = key;
            entry->value = value;
            /* state stays SLOT_OCCUPIED, size unchanged */
            return;
        }
    }

    /*
     * Probe wrapped fully without finding SLOT_EMPTY.
     * This can only happen if first_tombstone != -1 (map has space via
     * tombstones but no empty slot).  Insert at the tombstone.
     */
    if (first_tombstone != -1) {
        map->entries[first_tombstone].key   = key;
        map->entries[first_tombstone].value = value;
        map->entries[first_tombstone].state = SLOT_OCCUPIED;
        map->size++;
        return;
    }
    /* If we somehow got here with no tombstone the map is truly full.
     * Resize should have prevented this, but avoid leaking the new pair. */
    value_free(key);
    value_free(value);
}

/* Double capacity and re-insert only SLOT_OCCUPIED entries */
static void _hmap_resize(HMap *map) {
    if (map->capacity > INT_MAX / 2) return; /* overflow guard */
    int new_capacity = map->capacity * 2;
    HMapEntry *new_entries = malloc((size_t)new_capacity * sizeof(HMapEntry));
    if (new_entries == NULL) return;          /* allocation failure — keep old state */

    for (int i = 0; i < new_capacity; i++) {
        new_entries[i].state = SLOT_EMPTY;
        new_entries[i].key   = NULL;
        new_entries[i].value = NULL;
    }

    /* Temporarily swap to new array so _hmap_put_value probes into it */
    HMapEntry *old_entries  = map->entries;
    int        old_capacity = map->capacity;

    map->entries  = new_entries;
    map->capacity = new_capacity;
    map->size     = 0;          /* will be rebuilt by the re-inserts below */

    for (int i = 0; i < old_capacity; i++) {
        if (old_entries[i].state == SLOT_OCCUPIED) {
            /* Re-use the already-allocated key/value pointers */
            _hmap_put_value(map, old_entries[i].key, old_entries[i].value);
        }
        /* Tombstones are intentionally dropped — that's the whole point */
    }

    free(old_entries);
}

/* ===================================================================
 * Public API
 * =================================================================== */

HMap *HMap_new(void) {
    HMap *map = malloc(sizeof(HMap));
    if (map == NULL) return NULL;

    map->capacity = 16;
    map->size     = 0;
    map->entries  = malloc((size_t)map->capacity * sizeof(HMapEntry));
    if (map->entries == NULL) {
        free(map);
        return NULL;
    }

    for (int i = 0; i < map->capacity; i++) {
        map->entries[i].state = SLOT_EMPTY;
        map->entries[i].key   = NULL;
        map->entries[i].value = NULL;
    }

    return map;
}

void HMap_destroy(HMap *map) {
    if (map == NULL) return;
    hmap_clear(map);        /* frees all live key/value pairs */
    free(map->entries);
    free(map);
}

/* Returns the value pointer for key, or NULL if not found.
 * Does NOT transfer ownership — caller must NOT free the returned pointer. */
Value *hmap_get(HMap *map, Value *key) {
    if (map == NULL || key == NULL) return NULL;

    unsigned int idx = _hash_value(key, map->capacity);

    for (int i = 0; i < map->capacity; i++) {
        int probe = (int)((idx + (unsigned int)i) % (unsigned int)map->capacity);
        HMapEntry *entry = &map->entries[probe];

        if (entry->state == SLOT_EMPTY) {
            return NULL;   /* probe chain is clean — key absent */
        }
        if (entry->state == SLOT_TOMBSTONE) {
            continue;      /* skip deleted slot, keep probing */
        }
        /* SLOT_OCCUPIED */
        if (value_equals(entry->key, key)) {
            return entry->value;
        }
    }
    return NULL;
}

/* Returns 1 if the key was found and removed, 0 otherwise. */
int hmap_remove(HMap *map, Value *key) {
    if (map == NULL || key == NULL) return 0;

    unsigned int idx = _hash_value(key, map->capacity);

    for (int i = 0; i < map->capacity; i++) {
        int probe = (int)((idx + (unsigned int)i) % (unsigned int)map->capacity);
        HMapEntry *entry = &map->entries[probe];

        if (entry->state == SLOT_EMPTY) {
            return 0;   /* key not present */
        }
        if (entry->state == SLOT_TOMBSTONE) {
            continue;
        }
        /* SLOT_OCCUPIED */
        if (value_equals(entry->key, key)) {
            value_free(entry->key);
            value_free(entry->value);
            entry->key   = NULL;
            entry->value = NULL;
            entry->state = SLOT_TOMBSTONE;  /* NOT SLOT_EMPTY — preserves probe chains */
            map->size--;
            return 1;
        }
    }
    return 0;
}

int hmap_contains(HMap *map, Value *key) {
    return hmap_get(map, key) != NULL;
}

int hmap_size(HMap *map) {
    if (map == NULL) return 0;
    return map->size;
}

int hmap_is_empty(HMap *map) {
    if (map == NULL) return 1;
    return map->size == 0;
}

/* Frees all live key/value pairs; resets every slot to SLOT_EMPTY.
 * Does NOT free entries array or struct itself. */
void hmap_clear(HMap *map) {
    if (map == NULL) return;

    for (int i = 0; i < map->capacity; i++) {
        if (map->entries[i].state == SLOT_OCCUPIED) {
            value_free(map->entries[i].key);
            value_free(map->entries[i].value);
        }
        map->entries[i].key   = NULL;
        map->entries[i].value = NULL;
        map->entries[i].state = SLOT_EMPTY;
    }
    map->size = 0;
}

/* Prints: { key: value, key: value }
 * Skips SLOT_EMPTY and SLOT_TOMBSTONE slots. */
void hmap_print(HMap *map) {
    if (map == NULL) {
        printf("{ <null map> }\n");
        return;
    }

    printf("{ ");
    int printed = 0;
    for (int i = 0; i < map->capacity; i++) {
        if (map->entries[i].state == SLOT_OCCUPIED) {
            if (printed > 0) printf(", ");
            value_print(map->entries[i].key);
            printf(": ");
            value_print(map->entries[i].value);
            printed++;
        }
    }
    printf(" }\n");
}

/* ===================================================================
 * Type-specific put wrappers (16 combinations: 4 key types × 4 value types)
 * These create heap-allocated Values and forward to _hmap_put_value.
 * =================================================================== */

/* --- key = int --- */
void _hmap_put_int_int   (HMap *m, int k, int v)          { _hmap_put_value(m, make_int(k),    make_int(v));    }
void _hmap_put_int_double(HMap *m, int k, double v)        { _hmap_put_value(m, make_int(k),    make_double(v)); }
void _hmap_put_int_char  (HMap *m, int k, char v)          { _hmap_put_value(m, make_int(k),    make_char(v));   }
void _hmap_put_int_string(HMap *m, int k, const char *v)   { _hmap_put_value(m, make_int(k),    make_string(v)); }

/* --- key = double --- */
void _hmap_put_double_int   (HMap *m, double k, int v)         { _hmap_put_value(m, make_double(k), make_int(v));    }
void _hmap_put_double_double(HMap *m, double k, double v)       { _hmap_put_value(m, make_double(k), make_double(v)); }
void _hmap_put_double_char  (HMap *m, double k, char v)         { _hmap_put_value(m, make_double(k), make_char(v));   }
void _hmap_put_double_string(HMap *m, double k, const char *v)  { _hmap_put_value(m, make_double(k), make_string(v)); }

/* --- key = char --- */
void _hmap_put_char_int   (HMap *m, char k, int v)          { _hmap_put_value(m, make_char(k),   make_int(v));    }
void _hmap_put_char_double(HMap *m, char k, double v)        { _hmap_put_value(m, make_char(k),   make_double(v)); }
void _hmap_put_char_char  (HMap *m, char k, char v)          { _hmap_put_value(m, make_char(k),   make_char(v));   }
void _hmap_put_char_string(HMap *m, char k, const char *v)   { _hmap_put_value(m, make_char(k),   make_string(v)); }

/* --- key = string --- */
void _hmap_put_string_int   (HMap *m, const char *k, int v)         { _hmap_put_value(m, make_string(k), make_int(v));    }
void _hmap_put_string_double(HMap *m, const char *k, double v)       { _hmap_put_value(m, make_string(k), make_double(v)); }
void _hmap_put_string_char  (HMap *m, const char *k, char v)         { _hmap_put_value(m, make_string(k), make_char(v));   }
void _hmap_put_string_string(HMap *m, const char *k, const char *v)  { _hmap_put_value(m, make_string(k), make_string(v)); }
