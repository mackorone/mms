#ifndef HISTORY_H_
#define HISTORY_H_

#include <list>
#include <queue>
#include <stack>
#include <utility>

#include "Cell.h"

class History {

public:

    void initialize(int stm, Cell* origin); // Resets the history object
    int size(); // Returns the number of steps taken by the mouse

    // NOTE: These accessors mutate the underlying objects - be careful to only call them once!
    std::stack<Cell*> getCheckpointPath(); // Returns the path to the checkpoint cell
    std::stack<Cell*> getCheckpointStack(); // Returns the stack that was present at the most recent checkpoint

    void moved(Cell* movedTo);
    void stackUpdate(std::stack<Cell*> newStack);
    void modifiedCellsUpdate(std::list<std::pair<Cell*, int>> cells);

    void resetModifiedCells();

private:

    // TODO: Checkpoint???

    int m_stm; // The short term memory of the robot

    // Checkpoint objects
    Cell* m_checkpointCell; // TODO: Maybe just want to store the cell here? Much less space
    std::stack<Cell*> m_checkpointStack;

    // Queue of the path of the mouse
    std::queue<Cell*> m_path;

    // Queue of stacks for explore
    std::queue<std::stack<Cell*>> m_stacks;

    // Queue of ints counting the references for each stack in m_stacks
    std::list<int> m_stackReferenceCounts;

    // Queue of cells modified for each step
    std::queue<std::list<std::pair<Cell*, int>>> m_modifiedCells;

    // Queue of ints counting the references to each of the lists in m_modifiedCells
    std::list<int> m_modifiedCellsReferenceCounts; 

    Cell* getCheckpointCell(); // Returns a pointer the checkpoint cell

    // TODO
    void printS();
    void printC();
};

#endif // HISTORY_H_
