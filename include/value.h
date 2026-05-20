#ifndef VALUE_H
#define VALUE_H

#include <stdlib.h>

typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        int    i;
        double d;
        char   c;
        char  *s;
    } as;
} Value;

/* Constructors — caller owns the returned Value* and must call value_free(). */
Value *make_int(int val);
Value *make_double(double val);
Value *make_char(char val);
Value *make_string(const char *val);       /* copies the string internally */

void   value_print(const Value *v);         /* borrows — does NOT take ownership */
void   value_free(Value *v);                /* frees an owned Value* (safe to call on NULL) */
int    value_equals(const Value *a, const Value *b);  /* borrows both */
int    value_compare(const Value *a, const Value *b);  /* borrows both; <0 if a<b, 0 if equal, >0 if a>b */

#endif // VALUE_H
