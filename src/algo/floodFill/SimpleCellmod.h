#ifndef SIMPLECELLMOD_H_
#define SIMPLECELLMOD_H_

// Structure that contains modification information for the simple
// floodfiil algorithm implementation
struct SimpleCellmod {
    Cell* cell;
    Cell* prev;
    int dist;
    bool walls[4];
};

#endif // SIMPLECELLMOD_H_
