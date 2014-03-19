#include "History.h"

#include <iostream>

void History::initialize(int stm, Cell* origin) {

    // Set the short term memory
    m_stm = stm;

    // Erase any left-over data
    while (!m_path.empty()) {
        m_path.pop();
    }
    while (!m_stacks.empty()) {
        m_stacks.pop();
    }

    // Push the stack [(0,0)] since (0,0) or is *always* our first target
    std::stack<Cell*> temp;
    temp.push(origin);
    m_stacks.push(temp);
    m_stackReferenceCounts.push_back(1);

    // We *imagine* that we move to the next target (0,0). When the explore algorithm first
    // executes, it doesn't actually move to (0,0) since that is the starting location. However,
    // since the origin is part of the path, we need to make sure to push it to the path.
    m_path.push(origin);

    // Push an empty list of modified cells
    std::list<std::pair<Cell*, int>> empty;
    m_modifiedCells.push(empty);
    m_modifiedCellsReferenceCounts.push_back(1);
}

int History::size() {
    return m_path.size() - 1;
}

Cell* History::getRecoveryCell() {
    Cell* r = m_path.front();
    std::cout << "Recovery Cell (" << r->getX() << "," << r->getY() << ")" << std::endl;
    return m_path.front();
}

std::stack<Cell*> History::getRecoveryPath() {
    std::stack<Cell*> path;
    Cell* runner = getRecoveryCell();
    while (runner != NULL) {
        path.push(runner);
        runner = runner->getPrev(); 
    }
    return path;
}

std::stack<Cell*> History::getRecoveryStack() {
    // Print the stack
    std::stack<Cell*> temp = m_stacks.front();
    for (int i = 0; i < temp.size(); i++) {
        Cell* c = temp.top();
        temp.pop();
        std::cout << "(" << c->getX() << "," << c->getY() << ")" << std::endl; 
    }
    return m_stacks.front();
}

void History::moved(Cell* movedTo) {

    m_path.push(movedTo);

    int temp = m_stackReferenceCounts.back();
    m_stackReferenceCounts.pop_back();
    m_stackReferenceCounts.push_back(temp+1);

    if (m_path.size() > m_stm + 1) {

        m_path.pop(); 

        int temp = m_stackReferenceCounts.front();
        m_stackReferenceCounts.pop_front();
        m_stackReferenceCounts.push_front(temp-1);

        int temp2 = m_modifiedCellsReferenceCounts.front();
        m_modifiedCellsReferenceCounts.pop_front();
        m_modifiedCellsReferenceCounts.push_front(temp2-1);
    }
    if (m_stackReferenceCounts.front() == 0) {
        m_stackReferenceCounts.pop_front();
        m_stacks.pop();
    }
    if (m_modifiedCellsReferenceCounts.front() == 0) {
        m_modifiedCellsReferenceCounts.pop_front();
        m_modifiedCells.pop();
    }
}

void History::stackUpdate(std::stack<Cell*> newStack) {

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

void History::modifiedCellsUpdate(std::list<std::pair<Cell*, int>> cells) {

    // Analogous to stackUpdate
    m_modifiedCells.push(cells);
    int temp = m_modifiedCellsReferenceCounts.back();
    m_modifiedCellsReferenceCounts.pop_back();
    m_modifiedCellsReferenceCounts.push_back(temp-1);
    m_modifiedCellsReferenceCounts.push_back(1);
}

void History::resetModifiedCells() {
    while (!m_modifiedCells.empty()) {
        std::list<std::pair<Cell*, int>> cellList = m_modifiedCells.front();
        m_modifiedCells.pop();
        for (std::list<std::pair<Cell*, int>>::iterator it = cellList.begin(); it != cellList.end() ; ++it) {
            (std::get<0>(*it))->setPrev(NULL);
            (std::get<0>(*it))->setWallInspected(std::get<1>(*it), false);
        }
    }
}
