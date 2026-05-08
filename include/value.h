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

Value *make_int(int val);
Value *make_double(double val);
Value *make_char(char val);
Value *make_string(const char *val);
void   value_print(const Value *v);
void   value_free(Value *v);
int    value_equals(const Value *a, const Value *b);
int    value_compare(const Value *a, const Value *b); // Returns <0 if a < b, 0 if a == b, >0 if a > b

#endif // VALUE_H
