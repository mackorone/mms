#ifndef CELL_H_
#define CELL_H_

#include <stdbool.h>

// Defines struct Cell.

struct Cell {
    int m_x; // x-location of the Cell
    int m_y; // y-location of the Cell
    int m_distance;           // Distance of the Cell from the goal
    bool m_explored;          // Whether or not the Cell was traversed by the mouse
    bool m_walls[4];          // Whether or not a wall exists on the North, East, South, and West sides
    bool m_wallsInspected[4]; // Whether or not we've inspected the specific walls of a Cell
    struct Cell *m_prev;      // The previous node in the path by which the Cell was discovered
};

#endif // CELL_H_
