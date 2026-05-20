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

/* Lifecycle */
LinkedList *LinkedList_new(void);                       /* caller owns */
void        LinkedList_destroy(LinkedList *list);       /* frees list + all contained Values */

/* Type-specific push_front — list takes ownership of created Value */
void  _list_push_front_int    (LinkedList *list, int val);
void  _list_push_front_double (LinkedList *list, double val);
void  _list_push_front_char   (LinkedList *list, char val);
void  _list_push_front_string (LinkedList *list, const char *val);

/* Type-specific push_back — list takes ownership of created Value */
void  _list_push_back_int     (LinkedList *list, int val);
void  _list_push_back_double  (LinkedList *list, double val);
void  _list_push_back_char    (LinkedList *list, char val);
void  _list_push_back_string  (LinkedList *list, const char *val);

/* Type-specific insert_at — list takes ownership of created Value */
void  _list_insert_at_int     (LinkedList *list, int index, int val);
void  _list_insert_at_double  (LinkedList *list, int index, double val);
void  _list_insert_at_char    (LinkedList *list, int index, char val);
void  _list_insert_at_string  (LinkedList *list, int index, const char *val);

/* Type-specific update_at — list frees old Value, takes ownership of new */
void  _list_update_at_int     (LinkedList *list, int index, int val);
void  _list_update_at_double  (LinkedList *list, int index, double val);
void  _list_update_at_char    (LinkedList *list, int index, char val);
void  _list_update_at_string  (LinkedList *list, int index, const char *val);

/* Contains — borrows val for comparison only */
int   _list_contains_int      (LinkedList *list, int val);
int   _list_contains_double   (LinkedList *list, double val);
int   _list_contains_char     (LinkedList *list, char val);
int   _list_contains_string   (LinkedList *list, const char *val);

/* Access — returns BORROWED pointer; caller must NOT free */
Value  *list_peek_front  (LinkedList *list);
Value  *list_peek_back   (LinkedList *list);
Value  *list_get_at      (LinkedList *list, int index);

/* Removal — returns OWNED pointer; caller MUST call value_free() */
Value  *list_pop_front   (LinkedList *list);
Value  *list_pop_back    (LinkedList *list);

void    list_remove_at   (LinkedList *list, int index);  /* frees removed Value internally */
int     list_size        (LinkedList *list);
void    list_reverse     (LinkedList *list);
void    list_clear       (LinkedList *list);              /* frees all contained Values */
void    list_print       (LinkedList *list);

#endif // BEDROCK_LIST_H
