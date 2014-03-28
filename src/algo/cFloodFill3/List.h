#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>

struct ListNode {
    struct ListNode *ahead;
    struct ListNode *behind;
    void *data;
};

struct List {
    struct ListNode *front;
    struct ListNode *back;
};

struct List * newList(void);
void destroyList(struct List *);
bool isEmpty(struct List *);
void pop_front(struct List *);
void pop_back(struct List *);
void push_front(struct List *, void *)
void push_back(struct List *, void *)
void * front(struct List *);
void * back(struct List *);
size_t size(struct List *);

#endif // LIST_H_
