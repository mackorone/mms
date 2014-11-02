#ifndef CELLSTACK_H_
#define CELLSTACK_H_

#include "_95_Cell.h"

#include <stdbool.h>

struct CellStackNode {
    struct CellStackNode *above;
    struct CellStackNode *below;
    struct Cell *data;
};

struct CellStack {
    struct CellStackNode *top;
    struct CellStackNode *bottom;
};

struct CellStack * newStack(void);
void destroyStack(struct CellStack *);
void emptyStack(struct CellStack *); // Pops the stack until it's empty, but doesn't destroy it.
bool isEmpty(struct CellStack *);
void pop(struct CellStack *);
void push(struct CellStack *, struct Cell *);
struct Cell * top(struct CellStack *);

#endif // CELLSTACK_H_
