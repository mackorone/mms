#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>

struct ListNode {
    struct ListNode *ahead;
    struct ListNode *behind;
    void * data;
};

struct List {
    struct ListNode *front;
    struct ListNode *back;
};

struct List * createList(void);
void destroyList(struct List *);
bool isEmptyList(struct List *);
void pop_front(struct List *);
void pop_back(struct List *);
void push_front(struct List *, void *);
void push_back(struct List *, void *);
void * front(struct List *);
void * back(struct List *);
size_t size(struct List *);
struct List * copyOfList(struct List *);
void removeItem(struct List *, void *);

#endif // LIST_H_
