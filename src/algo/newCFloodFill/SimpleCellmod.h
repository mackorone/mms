#ifndef SIMPLECELLMOD_H_
#define SIMPLECELLMOD_H_

#include "Cell.h"

#include <stdbool.h>

// Structure that contains modification information for the simple
// floodfiil algorithm implementation
struct SimpleCellmod {
    struct Cell *cell;
    struct Cell *prev;
    int dist;
    bool walls[4];
};

struct SimpleCellmod * createSimpleCellmod(struct Cell *, struct Cell *, int indist, bool *);
struct SimpleCellmod * copyOfSimpleCellmod(struct SimpleCellmod *);
void destroySimpleCellmod (SimpleCellmod *);

#endif // SIMPLECELLMOD_H_
