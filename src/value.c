#include "../include/value.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

Value *make_int(int val) {
    Value *v = malloc(sizeof(Value));
    v->type = TYPE_INT;
    v->as.i = val;
    return v;
}

Value *make_double(double val) {
    Value *v = malloc(sizeof(Value));
    v->type = TYPE_DOUBLE;
    v->as.d = val;
    return v;
}

Value *make_char(char val) {
    Value *v = malloc(sizeof(Value));
    v->type = TYPE_CHAR;
    v->as.c = val;
    return v;
}

Value *make_string(const char *val) {
    Value *v = malloc(sizeof(Value));
    v->type = TYPE_STRING;
    size_t len = strlen(val) + 1;
    v->as.s = malloc(len);
    strcpy(v->as.s, val);
    return v;
}

void value_print(const Value *v) {
    if (v == NULL) return;
    
    switch (v->type) {
        case TYPE_INT:
            printf("%d", v->as.i);
            break;
        case TYPE_DOUBLE:
            printf("%g", v->as.d);
            break;
        case TYPE_CHAR:
            printf("'%c'", v->as.c);
            break;
        case TYPE_STRING:
            printf("\"%s\"", v->as.s);
            break;
    }
}

void value_free(Value *v) {
    if (v == NULL) return;
    
    if (v->type == TYPE_STRING && v->as.s != NULL) {
        free(v->as.s);
    }
    free(v);
}

int value_equals(const Value *a, const Value *b) {
    if (a == NULL || b == NULL) return 0;
    if (a->type != b->type) return 0;
    
    switch (a->type) {
        case TYPE_INT:
            return a->as.i == b->as.i;
        case TYPE_DOUBLE:
            return fabs(a->as.d - b->as.d) < 1e-9; // Use epsilon for floating-point comparison
        case TYPE_CHAR:
            return a->as.c == b->as.c;
        case TYPE_STRING:
            return strcmp(a->as.s, b->as.s) == 0;
    }
    
    return 0;
}

int value_compare(const Value *a, const Value *b) {
    if (a == NULL || b == NULL) return 0;
    
    // Type mismatch: TYPE_INT < TYPE_DOUBLE < TYPE_CHAR < TYPE_STRING (by enum order)
    if (a->type != b->type) {
        return (a->type < b->type) ? -1 : 1;
    }
    
    // Same type: compare by value
    switch (a->type) {
        case TYPE_INT:
            if (a->as.i < b->as.i) return -1;
            if (a->as.i > b->as.i) return 1;
            return 0;
        case TYPE_DOUBLE:
            if (fabs(a->as.d - b->as.d) < 1e-9) return 0;
            return (a->as.d < b->as.d) ? -1 : 1;
        case TYPE_CHAR:
            if (a->as.c < b->as.c) return -1;
            if (a->as.c > b->as.c) return 1;
            return 0;
        case TYPE_STRING:
            return strcmp(a->as.s, b->as.s);
    }
    
    return 0;
}
