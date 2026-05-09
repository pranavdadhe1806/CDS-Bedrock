#ifndef BEDROCK_HMAP_H
#define BEDROCK_HMAP_H

#include "value.h"

/* -----------------------------------------------------------------------
 * Hash Map  (Open Addressing · Linear Probing · Value-tagged keys/values)
 * ----------------------------------------------------------------------- */

typedef enum {
    SLOT_EMPTY,      /* never been used — probe can stop here              */
    SLOT_OCCUPIED,   /* live entry                                          */
    SLOT_TOMBSTONE   /* deleted — probing must continue past this slot     */
} SlotState;

typedef struct {
    Value    *key;
    Value    *value;
    SlotState state;
} HMapEntry;

typedef struct {
    HMapEntry *entries;   /* flat array of HMapEntry                       */
    int        capacity;  /* total allocated slots (always power-of-2)     */
    int        size;      /* live entries only (tombstones do NOT count)   */
} HMap;

/* -----------------------------------------------------------------------
 * Lifecycle
 * ----------------------------------------------------------------------- */
HMap *HMap_new(void);                    /* initial capacity = 16          */
void  HMap_destroy(HMap *map);           /* frees keys, values, array, struct */

/* -----------------------------------------------------------------------
 * Core operations
 * ----------------------------------------------------------------------- */
Value *hmap_get     (HMap *map, Value *key);           /* NULL if missing  */
int    hmap_remove  (HMap *map, Value *key);           /* 1=found, 0=miss  */
int    hmap_contains(HMap *map, Value *key);
int    hmap_size    (HMap *map);
int    hmap_is_empty(HMap *map);
void   hmap_clear   (HMap *map);                       /* keeps array alloc */
void   hmap_print   (HMap *map);

/* -----------------------------------------------------------------------
 * Type-specific put functions (called via the hmap_put macro in bedrock.h)
 * ----------------------------------------------------------------------- */

/* key = int */
void _hmap_put_int_int   (HMap *map, int k, int v);
void _hmap_put_int_double(HMap *map, int k, double v);
void _hmap_put_int_char  (HMap *map, int k, char v);
void _hmap_put_int_string(HMap *map, int k, const char *v);

/* key = double */
void _hmap_put_double_int   (HMap *map, double k, int v);
void _hmap_put_double_double(HMap *map, double k, double v);
void _hmap_put_double_char  (HMap *map, double k, char v);
void _hmap_put_double_string(HMap *map, double k, const char *v);

/* key = char */
void _hmap_put_char_int   (HMap *map, char k, int v);
void _hmap_put_char_double(HMap *map, char k, double v);
void _hmap_put_char_char  (HMap *map, char k, char v);
void _hmap_put_char_string(HMap *map, char k, const char *v);

/* key = string */
void _hmap_put_string_int   (HMap *map, const char *k, int v);
void _hmap_put_string_double(HMap *map, const char *k, double v);
void _hmap_put_string_char  (HMap *map, const char *k, char v);
void _hmap_put_string_string(HMap *map, const char *k, const char *v);

#endif /* BEDROCK_HMAP_H */
