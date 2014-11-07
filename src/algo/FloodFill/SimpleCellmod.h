#pragma once

// Structure that contains modification information for the simple
// floodfiil algorithm implementation
struct SimpleCellmod {
    Cell* cell;
    Cell* prev;
    int dist;
    bool walls[4];
};
