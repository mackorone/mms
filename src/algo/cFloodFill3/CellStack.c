#include "Cell.h"
#include "CellStack.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

struct CellStack * newStack(void) {
    struct CellStack *ptr = malloc(sizeof(struct CellStack));
    ptr->top = NULL;
    ptr->bottom = NULL;
    return ptr;
}

void destroyStack(struct CellStack *stack) { // Does not destroy Cells; only destroys stack structure.

    // Note: Invariants aren't preserved here, but it doesn't matter since we're destroying the stack.

    struct CellStackNode *top = stack->top;

    while (top != NULL) {
        if (top->below == NULL) {
            free(top);
            top = NULL;
        } else {
            top = top->below;
            free(top->above);
        }
    }

    free(stack);

}

void emptyStack(struct CellStack *stack) {
    while (!isEmpty(stack))
        pop(stack);
}

bool isEmpty(struct CellStack *stack) {
    return stack->bottom == NULL;
}

void pop(struct CellStack *stack) {

    // Does not do anything if the stack is empty.

    if (stack->bottom != NULL) {
        if (stack->bottom == stack->top) { // Only one element left in the stack.
            free(stack->bottom);
            stack->bottom = NULL;
            stack->top = NULL;
        } else {
            stack->top = stack->top->below;
            free(stack->top->above);
            stack->top->above = NULL;
        }
    }

}

void push(struct CellStack *stack, struct Cell *item) {

    struct CellStackNode *newNode = malloc(sizeof(struct CellStackNode));
    newNode->data = item;
    newNode->above = NULL;

    if (stack->bottom == NULL) { // The stack is empty.
        stack->top = newNode;
        stack->bottom = newNode;
        newNode->below = NULL;
    } else {
        stack->top->above = newNode;
        newNode->below = stack->top;
        stack->top = newNode;
    }

}

struct Cell * top(struct CellStack *stack) {
    return stack->top->data;
}

struct CellStack * copyOfStack(struct CellStack *original) {

    // NOTE: Does not copy struct Cell pointers (data field of nodes).
    // TODO: Is this a problem?

    // Returns a null pointer if a null pointer is passed in.
    if (original == NULL) return NULL;

    struct CellStack *copy = malloc(sizeof(struct CellStack));
    copy->top = NULL;
    copy->bottom = NULL;

    struct CellStackNode *curNode = original->bottom;
    while (curNode != NULL) {
        push(copy, curNode->data);
        curNode = curNode->above;
    }

    return copy;

}
