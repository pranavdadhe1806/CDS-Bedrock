#ifndef BEDROCK_H
#define BEDROCK_H

#include "dyn_array.h"
#include "bedrock_list.h"

// push — auto-detects type, user never thinks about it
#define push(arr, val) _Generic((val),      \
    int:         _push_int,                 \
    double:      _push_double,              \
    char:        _push_char,                \
    char*:       _push_string,              \
    const char*: _push_string               \
)(arr, val)

// Internal helper functions for insert/update (inline for efficiency)
static inline void _insert_int(DynArray *arr, int idx, int val) {
    Value *v = make_int(val);
    dyn_array_insert(arr, idx, v);
}

static inline void _insert_double(DynArray *arr, int idx, double val) {
    Value *v = make_double(val);
    dyn_array_insert(arr, idx, v);
}

static inline void _insert_char(DynArray *arr, int idx, char val) {
    Value *v = make_char(val);
    dyn_array_insert(arr, idx, v);
}

static inline void _insert_string(DynArray *arr, int idx, const char *val) {
    Value *v = make_string(val);
    dyn_array_insert(arr, idx, v);
}

static inline void _update_int(DynArray *arr, int idx, int val) {
    Value *v = make_int(val);
    dyn_array_update(arr, idx, v);
}

static inline void _update_double(DynArray *arr, int idx, double val) {
    Value *v = make_double(val);
    dyn_array_update(arr, idx, v);
}

static inline void _update_char(DynArray *arr, int idx, char val) {
    Value *v = make_char(val);
    dyn_array_update(arr, idx, v);
}

static inline void _update_string(DynArray *arr, int idx, const char *val) {
    Value *v = make_string(val);
    dyn_array_update(arr, idx, v);
}

#define insert(arr, idx, val) _Generic((val),      \
    int:         _insert_int,                      \
    double:      _insert_double,                   \
    char:        _insert_char,                     \
    char*:       _insert_string,                    \
    const char*: _insert_string                     \
)(arr, idx, val)

#define update(arr, idx, val) _Generic((val),      \
    int:         _update_int,                      \
    double:      _update_double,                   \
    char:        _update_char,                     \
    char*:       _update_string,                   \
    const char*: _update_string                     \
)(arr, idx, val)

// clean aliases so user writes get/pop/etc not dyn_array_get
#define get(arr, i)        dyn_array_get(arr, i)
#define pop(arr)           dyn_array_pop(arr)
#define delete(arr, i)     dyn_array_delete(arr, i)
#define size(arr)          dyn_array_size(arr)
#define clear(arr)         dyn_array_clear(arr)
#define print(arr)         dyn_array_print(arr)

// contains needs special handling - create temporary Value for comparison
#define contains(arr, val) _Generic((val),         \
    int:         _contains_int_val,                \
    double:      _contains_double_val,              \
    char:        _contains_char_val,               \
    char*:       _contains_string_val,             \
    const char*: _contains_string_val              \
)(arr, val)

// Internal helpers for contains
static inline int _contains_int_val(DynArray *arr, int val) {
    Value tmp = {TYPE_INT, {.i = val}};
    return dyn_array_contains(arr, &tmp);
}

static inline int _contains_double_val(DynArray *arr, double val) {
    Value tmp = {TYPE_DOUBLE, {.d = val}};
    return dyn_array_contains(arr, &tmp);
}

static inline int _contains_char_val(DynArray *arr, char val) {
    Value tmp = {TYPE_CHAR, {.c = val}};
    return dyn_array_contains(arr, &tmp);
}

static inline int _contains_string_val(DynArray *arr, const char *val) {
    Value tmp = {TYPE_STRING, {.s = (char*)val}};
    return dyn_array_contains(arr, &tmp);
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

#endif // BEDROCK_H
