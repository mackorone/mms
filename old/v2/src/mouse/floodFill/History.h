#pragma once

#include <list>
#include <queue>
#include <stack>
#include <utility>

#include "Cell.h"
#include "Cellmod.h"

namespace floodFill {

class History {

public:

    // Set the history object for the origin
    void initialize(int stm, Cell* origin);

    // Checkpoint accessors
    Cell* getCheckpointCell(); // Returns a pointer the checkpoint cell
    std::stack<Cell*> getCheckpointStack(); // Returns the stack that was present at the most recent checkpoint
    std::stack<Cell*> getCheckpointPath(); // Returns the path to the checkpoint cell

    // Called by Floodfill to update the history object
    void moved();
    void stackUpdate(std::stack<Cell*> newStack);
    void modifiedCellsUpdate(std::list<Cellmod> cells);

    // Resets the modifications to each of the cells stored in short term mem
    void resetModifiedCells();

private:

    // The short term memory of the robot
    int m_stm; 

    // The number of steps since reaching the last checkpoint
    int m_size;

    // Checkpoint objects
    Cell* m_checkpointCell;
    std::stack<Cell*> m_checkpointStack;

    // Queue of stacks for explore
    std::queue<std::stack<Cell*>> m_stacks;

    // Queue of ints counting the references for each stack in m_stacks
    std::list<int> m_stackReferenceCounts;

    // List of List of cell modification objects for each step.
    std::list<std::list<Cellmod>> m_modifiedCells;
};

} // namespace floodFill
