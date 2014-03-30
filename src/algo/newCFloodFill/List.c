#include "List.h"
#include "Cellmod.h"
#include "SimpleCellmod.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

bool isEmpty(struct List *list) {
    return list->back == NULL;
}

void pop_front(struct List *list) {

    // Does not do anything if the list is empty.

    if (list->back != NULL) {
        if (list->back == list->front) { // Only one element left in the list.
            free(list->back);
            list->back = NULL;
            list->front = NULL;
        } else {
            list->front = list->front->behind;
            free(list->front->ahead);
            list->front->ahead = NULL;
        }
    }

}

void pop_back(struct List *list) {

    // Does not do anything if the list is empty.

    if (list->front != NULL) {
        if (list->front == list->back) { // Only one element left in the list.
            free(list->front);
            list->front = NULL;
            list->back = NULL;
        } else {
            list->back = list->back->ahead;
            free(list->back->behind);
            list->back->behind = NULL;
        }
    }

}

void push_back(struct List *list, void *item) {

    struct ListNode *newNode = malloc(sizeof(struct ListNode));
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

void push_front(struct List *list, void *item) {

    struct ListNode *newNode = malloc(sizeof(struct ListNode));
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

struct List * createList() {
    struct List *ptr = malloc(sizeof(struct List));
    ptr->front = NULL;
    ptr->back = NULL;
    return ptr;
}

void destroyList(struct List *list, bool destroyData) {

    // Note: Invariants aren't preserved here, but it doesn't matter since we're destroying the stack.

    struct ListNode *front = list->front;

    while (front != NULL) {
        if (front->behind == NULL) {
            if (destroyData)
                free(front->data);
            free(front);
            front = NULL;
        } else {
            front = front->behind;
            if (destroyData)
                free(front->ahead->data);
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

struct List * copyOfList(struct List *original, int indicator) {

    // 0 - normal
    // 1 - Cellmod
    // 2 - SimpleCellmod

    // Returns a null pointer if a null pointer is passed in.
    if (original == NULL) return NULL;

    struct List *copy = malloc(sizeof(struct List));
    copy->front = NULL;
    copy->back = NULL;

    struct ListNode *curNode = original->back;
    while (curNode != NULL) {
        void *temp = curNode->data;
        if (indicator == 1) {
            temp = copyOfCellmod((struct Cellmod *)temp);

        }
        if (indicator == 2) {
            temp = copyOfSimpleCellmod((struct SimpleCellmod *)temp);
        }

        push_front(copy, temp);
        curNode = curNode->ahead;
    }

    return copy;

}

void removeItem(struct List * list, void * item) {

    if (list == NULL) return;

    struct ListNode * node = list->front;
    if (node == NULL) return;    

    while (true) {

        if (node->data == item) {

            if (list->back == list->front) { // Only one element left in the list.
                free(list->back);
                list->back = NULL;
                list->front = NULL;
            } else {

                if (node->ahead == NULL) {
                    list->front = node->behind;
                    list->front->ahead = NULL;
                else if (node->behind == NULL) {
                    list->back = node->ahead;
                    list->back->behind = NULL;
                } else {
                    node->behind->ahead = node->ahead;
                    node->ahead->behind = node->behind;
                }
    
                free(node);

            }
                        
            break;

        }

        if (node == list->back) {
            break;
        }
        node = node->behind;
    }

}
