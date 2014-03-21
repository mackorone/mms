#ifndef HISTORY_H_
#define HISTORY_H_

#include <list>
#include <queue>
#include <stack>
#include <utility>

#include "Cell.h"

class History {

public:

    void initialize(int stm, Cell* origin); // Set the history object for the origin

    std::stack<Cell*> getCheckpointStack(); // Returns the stack that was present at the most recent checkpoint
    std::stack<Cell*> getCheckpointPath(); // Returns the path to the checkpoint cell

    void moved(Cell* movedTo);
    void stackUpdate(std::stack<Cell*> newStack);
    void modifiedCellsUpdate(std::list<std::pair<Cell*, int>> cells);

    void resetModifiedCells();

private:

    // The short term memory of the robot
    int m_stm; 

    // Checkpoint objects
    Cell* m_checkpointCell;
    std::stack<Cell*> m_checkpointStack;

    // Queue of the path of the mouse
    std::queue<Cell*> m_path; // TODO: Get rid of this and just use int

    int size();
    // The number of steps since reaching the last checkpoint
    //int m_steps; // TODO

    // Queue of stacks for explore
    std::queue<std::stack<Cell*>> m_stacks;

    // Queue of ints counting the references for each stack in m_stacks
    std::list<int> m_stackReferenceCounts;

    // List of List of cells modified for each step. The first cell indicates the cell
    // that was modified, the second cell is the old prev value, and the int is the
    // wall that was modified.
    std::list<std::list<std::pair<Cell*, int>>> m_modifiedCells;

    Cell* getCheckpointCell(); // Returns a pointer the checkpoint cell

    // TODO
    void printS();
    void printC();
};

#endif // HISTORY_H_
