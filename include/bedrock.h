#ifndef BEDROCK_H
#define BEDROCK_H

#include "value.h"
#include "bedrock_array.h"
#include "bedrock_list.h"
#include "bedrock_stack.h"
#include "bedrock_queue.h"
#include "bedrock_bst.h"
#include "bedrock_hmap.h"
#include "bedrock_heap.h"
#include "bedrock_graph.h"

// ==================== ARRAY MACROS ====================

// push — auto-detects type, user never thinks about it
#define push(arr, val) _Generic((val),      \
    int:         _brarray_push_int,          \
    double:      _brarray_push_double,      \
    char:        _brarray_push_char,        \
    char*:       _brarray_push_string,      \
    const char*: _brarray_push_string       \
)(arr, val)

// Internal helper functions for insert/update (inline for efficiency)
static inline void _insert_int(BRArray *arr, int idx, int val) {
    Value *v = make_int(val);
    brarray_insert(arr, idx, v);
}

static inline void _insert_double(BRArray *arr, int idx, double val) {
    Value *v = make_double(val);
    brarray_insert(arr, idx, v);
}

static inline void _insert_char(BRArray *arr, int idx, char val) {
    Value *v = make_char(val);
    brarray_insert(arr, idx, v);
}

static inline void _insert_string(BRArray *arr, int idx, const char *val) {
    Value *v = make_string(val);
    brarray_insert(arr, idx, v);
}

static inline void _update_int(BRArray *arr, int idx, int val) {
    Value *v = make_int(val);
    brarray_update(arr, idx, v);
}

static inline void _update_double(BRArray *arr, int idx, double val) {
    Value *v = make_double(val);
    brarray_update(arr, idx, v);
}

static inline void _update_char(BRArray *arr, int idx, char val) {
    Value *v = make_char(val);
    brarray_update(arr, idx, v);
}

static inline void _update_string(BRArray *arr, int idx, const char *val) {
    Value *v = make_string(val);
    brarray_update(arr, idx, v);
}

#define insert(arr, idx, val) _Generic((val),      \
    int:         _insert_int,                      \
    double:      _insert_double,                   \
    char:        _insert_char,                     \
    char*:       _insert_string,                   \
    const char*: _insert_string                     \
)(arr, idx, val)

#define update(arr, idx, val) _Generic((val),      \
    int:         _update_int,                      \
    double:      _update_double,                   \
    char:        _update_char,                     \
    char*:       _update_string,                   \
    const char*: _update_string                     \
)(arr, idx, val)

// clean aliases so user writes get/pop/etc not brarray_get
#define get(arr, i)        brarray_get(arr, i)
#define pop(arr)           brarray_pop(arr)
#define delete(arr, i)     brarray_delete(arr, i)
#define size(arr)          brarray_size(arr)
#define clear(arr)         brarray_clear(arr)
#define print(arr)         brarray_print(arr)

// LinkedList aliases for uniform API
#define list_get(list, i)  list_get_at(list, i)
#define list_pop(list)     list_pop_front(list) // Default to front pop
#define list_delete(list,i) list_remove_at(list, i)
#define list_clear(list)   list_clear(list)
#define list_print(list)   list_print(list)

// Stack aliases for uniform API
#define stack_get(stack, i) brstack_peek(stack) // Stack only supports peek, not get by index
#define stack_pop(stack)    brstack_pop(stack)
#define stack_clear(stack)  brstack_clear(stack)
#define stack_print(stack)  brstack_print(stack)

// Queue aliases for uniform API
#define queue_get(queue, i) brqueue_peek(queue) // Queue only supports peek, not get by index
#define queue_pop(queue)    brqueue_dequeue(queue)
#define queue_clear(queue)  brqueue_clear(queue)
#define queue_print(queue)  brqueue_print(queue)

// contains needs special handling - create temporary Value for comparison
#define contains(arr, val) _Generic((val),         \
    int:         _contains_int_val,                \
    double:      _contains_double_val,            \
    char:        _contains_char_val,               \
    char*:       _contains_string_val,            \
    const char*: _contains_string_val              \
)(arr, val)

// Internal helpers for contains
static inline int _contains_int_val(BRArray *arr, int val) {
    Value tmp = {TYPE_INT, {.i = val}};
    return brarray_contains(arr, &tmp);
}

static inline int _contains_double_val(BRArray *arr, double val) {
    Value tmp = {TYPE_DOUBLE, {.d = val}};
    return brarray_contains(arr, &tmp);
}

static inline int _contains_char_val(BRArray *arr, char val) {
    Value tmp = {TYPE_CHAR, {.c = val}};
    return brarray_contains(arr, &tmp);
}

static inline int _contains_string_val(BRArray *arr, const char *val) {
    Value tmp = {TYPE_STRING, {.s = (char*)val}};
    return brarray_contains(arr, &tmp);
}

// ==================== LINKED LIST MACROS ====================

// push_front — auto-detects type
#define push_front(list, val) _Generic((val),   \
    int:         _list_push_front_int,          \
    double:      _list_push_front_double,       \
    char:        _list_push_front_char,         \
    char*:       _list_push_front_string,       \
    const char*: _list_push_front_string        \
)(list, val)

// push_back — auto-detects type
#define push_back(list, val) _Generic((val),    \
    int:         _list_push_back_int,           \
    double:      _list_push_back_double,       \
    char:        _list_push_back_char,          \
    char*:       _list_push_back_string,        \
    const char*: _list_push_back_string         \
)(list, val)

// insert_at — auto-detects type of value
#define insert_at(list, i, val) _Generic((val), \
    int:         _list_insert_at_int,           \
    double:      _list_insert_at_double,        \
    char:        _list_insert_at_char,          \
    char*:       _list_insert_at_string,        \
    const char*: _list_insert_at_string         \
)(list, i, val)

// update_at — auto-detects type of new value
#define update_at(list, i, val) _Generic((val), \
    int:         _list_update_at_int,           \
    double:      _list_update_at_double,        \
    char:        _list_update_at_char,          \
    char*:       _list_update_at_string,        \
    const char*: _list_update_at_string         \
)(list, i, val)

// contains for LinkedList — auto-detects type
#define list_contains(list, val) _Generic((val),     \
    int:         _list_contains_int,                \
    double:      _list_contains_double,         \
    char:        _list_contains_char,           \
    char*:       _list_contains_string,         \
    const char*: _list_contains_string          \
)(list, val)

// single-type aliases — no _Generic needed
#define peek_front(list)     list_peek_front(list)
#define peek_back(list)      list_peek_back(list)
#define pop_front(list)      list_pop_front(list)
#define pop_back(list)       list_pop_back(list)
#define get_at(list, i)      list_get_at(list, i)
#define remove_at(list, i)   list_remove_at(list, i)
#define list_reverse(list)   list_reverse(list)
#define list_size(list)      list_size(list)
#define list_clear(list)     list_clear(list)
#define list_print(list)     list_print(list)

// ==================== STACK MACROS ====================

// stack_push — auto-detects type
#define stack_push(stack, val) _Generic((val),   \
    int:         _brstack_push_int,             \
    double:      _brstack_push_double,         \
    char:        _brstack_push_char,            \
    char*:       _brstack_push_string,          \
    const char*: _brstack_push_string           \
)(stack, val)

// single-type aliases
#define stack_pop(stack)       brstack_pop(stack)
#define stack_peek(stack)      brstack_peek(stack)
#define stack_is_empty(stack)  brstack_is_empty(stack)
#define stack_size(stack)      brstack_size(stack)
#define stack_clear(stack)     brstack_clear(stack)
#define stack_print(stack)     brstack_print(stack)

// ==================== QUEUE MACROS ====================

// enqueue — auto-detects type
#define enqueue(queue, val) _Generic((val),    \
    int:         _brqueue_enqueue_int,          \
    double:      _brqueue_enqueue_double,      \
    char:        _brqueue_enqueue_char,        \
    char*:       _brqueue_enqueue_string,      \
    const char*: _brqueue_enqueue_string        \
)(queue, val)

// single-type aliases
#define queue_dequeue(queue)   brqueue_dequeue(queue)
#define queue_peek(queue)      brqueue_peek(queue)
#define queue_is_empty(queue)  brqueue_is_empty(queue)
#define queue_size(queue)      brqueue_size(queue)
#define queue_clear(queue)     brqueue_clear(queue)
#define queue_print(queue)     brqueue_print(queue)

// ==================== BST MACROS ====================

// bst_insert — auto-detects type
#define bst_insert(tree, val) _Generic((val),   \
    int:         _brbst_insert_int,              \
    double:      _brbst_insert_double,           \
    char:        _brbst_insert_char,             \
    char*:       _brbst_insert_string,           \
    const char*: _brbst_insert_string            \
)(tree, val)

// Internal helpers for search/remove (create temporary Value for comparison)
static inline int _brbst_search_int(BRBST *tree, int val) {
    Value tmp = {TYPE_INT, {.i = val}};
    return brbst_search(tree, &tmp);
}

static inline int _brbst_search_double(BRBST *tree, double val) {
    Value tmp = {TYPE_DOUBLE, {.d = val}};
    return brbst_search(tree, &tmp);
}

static inline int _brbst_search_char(BRBST *tree, char val) {
    Value tmp = {TYPE_CHAR, {.c = val}};
    return brbst_search(tree, &tmp);
}

static inline int _brbst_search_string(BRBST *tree, const char *val) {
    Value tmp = {TYPE_STRING, {.s = (char*)val}};
    return brbst_search(tree, &tmp);
}

static inline void _brbst_remove_int(BRBST *tree, int val) {
    Value tmp = {TYPE_INT, {.i = val}};
    brbst_remove(tree, &tmp);
}

static inline void _brbst_remove_double(BRBST *tree, double val) {
    Value tmp = {TYPE_DOUBLE, {.d = val}};
    brbst_remove(tree, &tmp);
}

static inline void _brbst_remove_char(BRBST *tree, char val) {
    Value tmp = {TYPE_CHAR, {.c = val}};
    brbst_remove(tree, &tmp);
}

static inline void _brbst_remove_string(BRBST *tree, const char *val) {
    Value tmp = {TYPE_STRING, {.s = (char*)val}};
    brbst_remove(tree, &tmp);
}

#define bst_contains(tree, val) _Generic((val), \
    int:         _brbst_search_int,              \
    double:      _brbst_search_double,           \
    char:        _brbst_search_char,             \
    char*:       _brbst_search_string,           \
    const char*: _brbst_search_string            \
)(tree, val)

#define bst_delete(tree, val) _Generic((val),   \
    int:         _brbst_remove_int,              \
    double:      _brbst_remove_double,           \
    char:        _brbst_remove_char,             \
    char*:       _brbst_remove_string,           \
    const char*: _brbst_remove_string            \
)(tree, val)

// BST aliases
#define bst_size(tree)      brbst_size(tree)
#define bst_is_empty(tree)  brbst_is_empty(tree)
#define bst_clear(tree)     brbst_clear(tree)
#define bst_height(tree)    brbst_height(tree)
#define bst_find_min(tree)  brbst_find_min(tree)
#define bst_find_max(tree)  brbst_find_max(tree)
#define bst_inorder(tree)   brbst_inorder(tree)
#define bst_preorder(tree)  brbst_preorder(tree)
#define bst_postorder(tree) brbst_postorder(tree)

// ==================== HASH MAP MACROS ====================

/*
 * hmap_put — nested _Generic: dispatch on key type first, then value type.
 * Resolves to one of the 16 _hmap_put_*_* functions.
 */
#define hmap_put(map, key, val) _Generic((key),                         \
    int:         _Generic((val),                                         \
                     int:         _hmap_put_int_int,                     \
                     double:      _hmap_put_int_double,                  \
                     char:        _hmap_put_int_char,                    \
                     char*:       _hmap_put_int_string,                  \
                     const char*: _hmap_put_int_string),                 \
    double:      _Generic((val),                                         \
                     int:         _hmap_put_double_int,                  \
                     double:      _hmap_put_double_double,               \
                     char:        _hmap_put_double_char,                 \
                     char*:       _hmap_put_double_string,               \
                     const char*: _hmap_put_double_string),              \
    char:        _Generic((val),                                         \
                     int:         _hmap_put_char_int,                    \
                     double:      _hmap_put_char_double,                 \
                     char:        _hmap_put_char_char,                   \
                     char*:       _hmap_put_char_string,                 \
                     const char*: _hmap_put_char_string),                \
    char*:       _Generic((val),                                         \
                     int:         _hmap_put_string_int,                  \
                     double:      _hmap_put_string_double,               \
                     char:        _hmap_put_string_char,                 \
                     char*:       _hmap_put_string_string,               \
                     const char*: _hmap_put_string_string),              \
    const char*: _Generic((val),                                         \
                     int:         _hmap_put_string_int,                  \
                     double:      _hmap_put_string_double,               \
                     char:        _hmap_put_string_char,                 \
                     char*:       _hmap_put_string_string,               \
                     const char*: _hmap_put_string_string)               \
)(map, key, val)

/*
 * hmap_lookup helpers — build a stack-allocated Value for comparison only.
 * The address is passed to hmap_get which never stores it, so this is safe.
 */
static inline Value *_hmap_get_int(HMap *map, int k) {
    Value tmp = {TYPE_INT, {.i = k}};
    return hmap_get(map, &tmp);
}

static inline Value *_hmap_get_double(HMap *map, double k) {
    Value tmp = {TYPE_DOUBLE, {.d = k}};
    return hmap_get(map, &tmp);
}

static inline Value *_hmap_get_char(HMap *map, char k) {
    Value tmp = {TYPE_CHAR, {.c = k}};
    return hmap_get(map, &tmp);
}

static inline Value *_hmap_get_string(HMap *map, const char *k) {
    Value tmp = {TYPE_STRING, {.s = (char *)k}};
    return hmap_get(map, &tmp);
}

#define hmap_lookup(map, key) _Generic((key),   \
    int:         _hmap_get_int,                 \
    double:      _hmap_get_double,              \
    char:        _hmap_get_char,                \
    char*:       _hmap_get_string,              \
    const char*: _hmap_get_string               \
)(map, key)

/*
 * hmap_delete helpers — same stack-Value trick as above.
 */
static inline int _hmap_remove_int(HMap *map, int k) {
    Value tmp = {TYPE_INT, {.i = k}};
    return hmap_remove(map, &tmp);
}

static inline int _hmap_remove_double(HMap *map, double k) {
    Value tmp = {TYPE_DOUBLE, {.d = k}};
    return hmap_remove(map, &tmp);
}

static inline int _hmap_remove_char(HMap *map, char k) {
    Value tmp = {TYPE_CHAR, {.c = k}};
    return hmap_remove(map, &tmp);
}

static inline int _hmap_remove_string(HMap *map, const char *k) {
    Value tmp = {TYPE_STRING, {.s = (char *)k}};
    return hmap_remove(map, &tmp);
}

#define hmap_delete(map, key) _Generic((key),   \
    int:         _hmap_remove_int,              \
    double:      _hmap_remove_double,           \
    char:        _hmap_remove_char,             \
    char*:       _hmap_remove_string,           \
    const char*: _hmap_remove_string            \
)(map, key)

// ==================== HEAP MACROS ====================

#define heap_insert(heap, val) _Generic((val),  \
    int:         _heap_insert_int,              \
    double:      _heap_insert_double,           \
    char:        _heap_insert_char,             \
    char*:       _heap_insert_string,           \
    const char*: _heap_insert_string            \
)(heap, val)

#define heap_top(heap)      heap_peek(heap)
#define heap_pop(heap)      heap_extract(heap)
#define heap_len(heap)      heap_size(heap)
#define heap_empty(heap)    heap_is_empty(heap)

#endif // BEDROCK_H
