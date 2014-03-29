#include "List.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

void destroyDoNothing(void *item) { }
void * copyDoNothing(void *item) { return item; }

bool isEmpty(struct List *list) {
    return list->back == NULL;
}

void pop_front(struct List *list, void (*destroyFunc)(void *)) {

    // Does not do anything if the list is empty.

    if (list->back != NULL) {
        if (list->back == list->front) { // Only one element left in the list.
            destroyFunc(list->back->data);
            free(list->back);
            list->back = NULL;
            list->front = NULL;
        } else {
            list->front = list->front->behind;
            destroyFunc(list->front->ahead->data);
            free(list->front->ahead);
            list->front->ahead = NULL;
        }
    }

}

void pop_back(struct List *list, void (*destroyFunc)(void *)) {

    // Does not do anything if the list is empty.

    if (list->front != NULL) {
        if (list->front == list->back) { // Only one element left in the list.
            destroyFunc(list->front->data);
            free(list->front);
            list->front = NULL;
            list->back = NULL;
        } else {
            list->back = list->back->ahead;
            destroyFunc(list->back->behind->data);
            free(list->back->behind);
            list->back->behind = NULL;
        }
    }

}

void push_back(struct List *list, void *item, void * (*copyFunc)(void *)) {

    struct ListNode *newNode = malloc(sizeof(struct ListNode));
    item = copyFunc(item);
    newNode->data = item;
    newNode->behind = NULL;

    if (list->back == NULL) { // The list is empty.
        list->front = newNode;
        newNode->ahead = NULL;
    } else {
        list->back->behind = newNode;
        newNode->ahead = list->back;
    }

    list->back = newNode;

}

void push_front(struct List *list, void *item, void * (*copyFunc)(void *)) {

    struct ListNode *newNode = malloc(sizeof(struct ListNode));
    item = copyFunc(item);
    newNode->data = item;
    newNode->ahead = NULL;

    if (list->front == NULL) { // The list is empty.
        list->back = newNode;
        newNode->behind = NULL;
    } else {
        list->front->ahead = newNode;
        newNode->behind = list->front;
    }

    list->front = newNode;

}

void * front(struct List *list) {
    return list->front->data;
}

void * back(struct List *list) {
    return list->back->data;
}

struct List * newList() {
    struct List *ptr = malloc(sizeof(struct List));
    ptr->front = NULL;
    ptr->back = NULL;
    return ptr;
}

void destroyList(struct List *list) {

    // Note: Invariants aren't preserved here, but it doesn't matter since we're destroying the stack.

    struct ListNode *front = list->front;

    while (front != NULL) {
        if (front->behind == NULL) {
            free(front);
            front = NULL;
        } else {
            front = front->behind;
            free(front->ahead);
        }
    }

    free(list);

}

size_t size(struct List *list) {

    size_t count = 0;

    struct ListNode *curNode = list->back;

    while (curNode != NULL) {
        count++;
        curNode = curNode->ahead;
    }

    return count;

}
