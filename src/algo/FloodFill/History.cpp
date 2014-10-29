#include "History.h"

#include <iostream>

void History::initialize(int stm, Cell* origin) {

    // Set the short term memory
    m_stm = stm;

    // Erase any left-over data
    m_size = 0;
    while (!m_stacks.empty()) {
        m_stacks.pop();
    }
    while (!m_stackReferenceCounts.empty()) {
        m_stackReferenceCounts.pop_front();
    }
    while (!m_modifiedCells.empty()) {
        m_modifiedCells.pop_front();
    }

    // Push the stack [(0,0)] since (0,0) or is *always* our first target
    std::stack<Cell*> temp;
    temp.push(origin);
    m_stacks.push(temp);
    m_stackReferenceCounts.push_back(1);

    // Set the checkpoint values
    m_checkpointCell = origin;
    m_checkpointStack = temp;
}

Cell* History::getCheckpointCell() {

    // The checkpoint cell is the prev cell for the cell on top of the stack for the 
    // frontmost (oldest) stack that we are storing within the m_stacks queue.
    // Note that although we will have the same checkpoint cell if the top of
    // the stack contains either the origin (0,0) or a neighbor of the origin, but
    // these two situations are differentiated from one another by m_checkpointStack
    // itself.
    
    return m_checkpointCell;
}

std::stack<Cell*> History::getCheckpointPath() {

    // Returns a path from the origin to the checkpoint (exclusive) as a stack

    std::stack<Cell*> path;
    Cell* runner = getCheckpointCell();

    while (runner != NULL) {
        path.push(runner);
        runner = runner->getPrev(); 
    }

    // The path always includes the origin, and since we can't and don't want 
    // to try to move to the origin, we can *always* pop it off safely
    path.pop();

    return path;
}

std::stack<Cell*> History::getCheckpointStack() {

    return m_checkpointStack;
}

void History::moved() {

    m_size++;

    // Assertion - Neither m_stacks nor m_stackReferenceCounts should ever be empty
    if (m_stackReferenceCounts.empty() || m_stacks.empty()) {
        std::cout << "Error - History object has zero stack references" << std::endl;
        exit(0);
    }

    // Increment our stack reference counts
    int srefCount = m_stackReferenceCounts.back();
    m_stackReferenceCounts.pop_back();
    m_stackReferenceCounts.push_back(srefCount+1);

    // Every move we push an empty list onto the list of lists of modified cells
    std::list<Cellmod> empty;
    m_modifiedCells.push_front(empty);

    // If the size of the path is larger than our short term memory (which it needn't
    // be) then reduce the reference counts and pop the appropriate number of things
    // off of the stacks.
    if (m_size > m_stm) {

        m_size--;

        int srefCount2 = m_stackReferenceCounts.front();
        m_stackReferenceCounts.pop_front();
        m_stackReferenceCounts.push_front(srefCount2-1);
    }
    if (m_stackReferenceCounts.front() == 0) {
        m_stackReferenceCounts.pop_front();
        m_stacks.pop();
        m_checkpointStack = m_stacks.front();
        if (m_stacks.front().top()->getPrev() != NULL) {
            m_checkpointCell = m_stacks.front().top()->getPrev();
        }
        else {
            std::cout << "Error - checkpoint has NULL prev Cell" << std::endl;
            exit(0);
        }
    }

    // Also make sure to only keep the correct number of modified cells
    if (m_modifiedCells.size() > m_stm) {
        m_modifiedCells.pop_back();
    }
}

void History::stackUpdate(std::stack<Cell*> newStack) {

    // Assertion - Neither m_stacks nor m_stackReferenceCounts should ever be empty
    if (m_stackReferenceCounts.empty() || m_stacks.empty()) {
        std::cout << "Error - History object has zero stack references" << std::endl;
        exit(0);
    }

    // When we perform the stack update, it's always after a cell has been pushed
    // to the path queue. However, in pushing the new Cell, we incremented the
    // reference count for that particular stack. Thus we need to decrement it
    // and then push the proper stack on m_stacks, as well as a reference count
    // of 1 for the new stack on the stack reference count queue
    m_stacks.push(newStack);

    int temp = m_stackReferenceCounts.back();
    m_stackReferenceCounts.pop_back();
    m_stackReferenceCounts.push_back(temp-1);
    m_stackReferenceCounts.push_back(1);
}

void History::modifiedCellsUpdate(std::list<Cellmod> cells) {

    // The only situation when this list will be empty is immediately after
    // returning to the origin after undo is called. This is because the checkpoint
    // stack will contain the origin, and thus won't move from the origin to get to
    // the target (the origin) but WILL perform the appropriate updates.
    if (m_modifiedCells.size() > 0) {
        m_modifiedCells.pop_front();
    }
    m_modifiedCells.push_front(cells);
}

void History::resetModifiedCells() {

    std::list<std::list<Cellmod>> temp = m_modifiedCells;

    // Iterate through all modified cells, starting with most recent and going
    // to least recent. During iterations, we simply restore the old values
    while (!temp.empty()) {

        std::list<Cellmod> cellList = temp.front();
        temp.pop_front();

        for (std::list<Cellmod>::iterator it = cellList.begin(); it != cellList.end() ; ++it) {
            (*it).cell->setPrev((*it).oldPrev);
            (*it).cell->setDistance((*it).oldDist);
            (*it).cell->setExplored((*it).oldExplored);
            (*it).cell->setTraversed((*it).oldTraversed);
            for (int i = 0; i < 4; i += 1) {
                (*it).cell->setWall(i, (*it).oldWalls[i]);
                (*it).cell->setWallInspected(i, (*it).oldWallsInspected[i]);
            }
        }
    }

    // After we get a new checkpoint, we empty everything
    m_size = 0;
    while (!m_stacks.empty()) {
        m_stacks.pop();
    }
    while (!m_stackReferenceCounts.empty()) {
        m_stackReferenceCounts.pop_front();
    }
    while (!m_modifiedCells.empty()) {
        m_modifiedCells.pop_front();
    }

    m_stacks.push(m_checkpointStack);
    m_stackReferenceCounts.push_back(1);
}
