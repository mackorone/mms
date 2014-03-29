#ifndef MSTACKSQUEUE_H_
#define MSTACKSQUEUE_H_

#include <stdbool.h>

#include "CellStack.h"

struct MStacksQueueNode {
    struct MStacksQueueNode *ahead;
    struct MStacksQueueNode *behind;
    struct CellStack *data;
};

struct MStacksQueue {
    struct MStacksQueueNode *front;
    struct MStacksQueueNode *back;
};

// TODO: Pass in a pointer rather than the value.

bool isEmptyMSQ(struct MStacksQueue);
void popMSQ(struct MStacksQueue);
void pushMSQ(struct MStacksQueue, struct CellStack *);
struct CellStack * frontMSQ(struct MStacksQueue);

#endif // MSTACKSQUEUE_H_
