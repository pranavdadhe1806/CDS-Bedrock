#ifndef BEDROCK_LIST_H
#define BEDROCK_LIST_H

#include "value.h"

typedef struct BRNode {
    Value       *data;
    struct BRNode *prev;
    struct BRNode *next;
} BRNode;

typedef struct {
    BRNode *head;
    BRNode *tail;
    int     size;
} LinkedList;

LinkedList *LinkedList_new(void);
void        LinkedList_destroy(LinkedList *list);

void  _list_push_front_int    (LinkedList *list, int val);
void  _list_push_front_double (LinkedList *list, double val);
void  _list_push_front_char   (LinkedList *list, char val);
void  _list_push_front_string (LinkedList *list, const char *val);

void  _list_push_back_int     (LinkedList *list, int val);
void  _list_push_back_double  (LinkedList *list, double val);
void  _list_push_back_char    (LinkedList *list, char val);
void  _list_push_back_string  (LinkedList *list, const char *val);

void  _list_insert_at_int     (LinkedList *list, int index, int val);
void  _list_insert_at_double  (LinkedList *list, int index, double val);
void  _list_insert_at_char    (LinkedList *list, int index, char val);
void  _list_insert_at_string  (LinkedList *list, int index, const char *val);

void  _list_update_at_int     (LinkedList *list, int index, int val);
void  _list_update_at_double  (LinkedList *list, int index, double val);
void  _list_update_at_char    (LinkedList *list, int index, char val);
void  _list_update_at_string  (LinkedList *list, int index, const char *val);

int   _list_contains_int      (LinkedList *list, int val);
int   _list_contains_double   (LinkedList *list, double val);
int   _list_contains_char     (LinkedList *list, char val);
int   _list_contains_string   (LinkedList *list, const char *val);

Value  *list_peek_front  (LinkedList *list);
Value  *list_peek_back   (LinkedList *list);
void    list_pop_front   (LinkedList *list); // Changed to void to prevent memory leaks
void    list_pop_back    (LinkedList *list); // Changed to void to prevent memory leaks
Value  *list_get_at      (LinkedList *list, int index);
void    list_remove_at   (LinkedList *list, int index);
int     list_size        (LinkedList *list);
void    list_reverse     (LinkedList *list);
void    list_clear       (LinkedList *list);
void    list_print       (LinkedList *list);

#endif // BEDROCK_LIST_H
