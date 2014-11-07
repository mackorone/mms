#pragma once

/*
 * This struct contains all of the information relevant to a cell modification.
 * We use this structure as opposed to another Cell object since we need less
 * information in this object than we do a Cell, and we want to use as little
 * space as possible.
 */
struct Cellmod {

    Cell* cell; // A pointer to the particular cell that has been modified

    // NOTE: All of these fields correspond to the state of the Cell prior to being modified
    Cell* oldPrev; // The old prev cell of the cell
    int oldDist; // The old distance of the cell
    bool oldExplored; // The old exploredness of the cell (not sure if we need or if we can set to false by default)
    bool oldTraversed; // The old traversedness of the cell (again, not sure if this is necessary)
    bool oldWalls[4]; // Whether or not a wall exists on the North, East, South, and West walls
    bool oldWallsInspected[4]; // Whether or not we've inspected the specific walls of a Cell

};
