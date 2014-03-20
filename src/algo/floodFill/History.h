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
    std::stack<Cell*> getRecoveryPath(); // Returns the path to the recovery cell
    std::stack<Cell*> getRecoveryStack(); // Returns the stack that was present m_stm steps ago

    void moved(Cell* movedTo);
    void stackUpdate(std::stack<Cell*> newStack);
    void modifiedCellsUpdate(std::list<std::pair<Cell*, int>> cells);

    void resetModifiedCells();

private:

    // TODO: Checkpoint???

    int m_stm; // The short term memory of the robot
    std::queue<Cell*> m_path; // List of the path of the mouse
    std::queue<std::stack<Cell*>> m_stacks; // Queue of stacks for explore
    std::list<int> m_stackReferenceCounts; // Queue of ints counting the references for each stack in m_stacks
    std::queue<std::list<std::pair<Cell*, int>>> m_modifiedCells; // Queue of cells modified for each step
    std::list<int> m_modifiedCellsReferenceCounts; // Queue of ints counting the references to each of the lists in m_modifiedCells

    Cell* getRecoveryCell(); // Returns a pointer the recovery cell


    // TODO
    void printS();
    void printC();
};

#endif // HISTORY_H_
