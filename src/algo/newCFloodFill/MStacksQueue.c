#include "CellStack.h"
#include "MStacksQueue.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

bool isEmptyMSQ(struct MStacksQueue queue) {
    return queue.back == NULL;
}

void popMSQ(struct MStacksQueue queue) {

    // Does not do anything if the queue is empty.

    if (queue.back != NULL) {
        if (queue.back == queue.front) { // Only one element left in the stack.
            destroyStack(queue.back->data);
            free(queue.back);
            queue.back = NULL;
            queue.front = NULL;
        } else {
            queue.front = queue.front->behind;
            destroyStack(queue.front->ahead->data);
            free(queue.front->ahead);
            queue.front->ahead = NULL;
        }
    }

}

void pushMSQ(struct MStacksQueue queue, struct CellStack *item) {

    struct MStacksQueueNode *newNode = malloc(sizeof(struct MStacksQueueNode));
    newNode->data = copyOfStack(item);
    newNode->behind = NULL;

    if (queue.back == NULL) { // The stack is empty.
        queue.front = newNode;
        newNode->ahead = NULL;
    } else {
        queue.back->behind = newNode;
        newNode->ahead = queue.back;
    }

    queue.back = newNode;

}

struct CellStack * frontMSQ(struct MStacksQueue queue) {
    return copyOfStack(queue.front->data); // Copy here is unavoidable.  Otherwise it gets messy when we pop.
}
