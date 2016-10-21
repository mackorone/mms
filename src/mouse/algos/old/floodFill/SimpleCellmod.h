#pragma once

namespace floodFill {

// Structure that contains modification information for the simple
// floodfiil algorithm implementation
struct SimpleCellmod {
    Cell* cell;
    Cell* prev;
    int dist;
    bool walls[4];
};

} // namespace floodFill
