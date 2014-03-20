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
    while (!m_stackReferenceCounts.empty()) {
        m_stackReferenceCounts.pop_front();
    }
    while (!m_modifiedCells.empty()) {
        m_modifiedCells.pop();
    }
    while (!m_modifiedCellsReferenceCounts.empty()) {
        m_modifiedCellsReferenceCounts.pop_front();
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

    // Set the checkpoint values
    m_checkpointCell = origin;
    m_checkpointStack = temp;

    // Push an empty list of modified cells as a dummy node
    std::list<std::pair<Cell*, int>> empty;
    m_modifiedCells.push(empty);
    m_modifiedCellsReferenceCounts.push_back(1);
}

int History::size() {
    return m_path.size() - 1;
}

// TODO: Consolidate checkpoint cell and stack into a single pair
Cell* History::getCheckpointCell() {

    // Check to see if we've moved enough steps after an undo request was made to
    // advance the checkpoint. If so, update the checkpoint cell.
    // If not, simply return the last checkpoint.
    if (size() == m_stm) {

        // The checkpoint cell is the prev cell for the cell on top of the stack for the 
        // furthest back stack that we are storing within m_stacks
        Cell* r = m_stacks.front().top();
        r = r->getPrev();
        m_checkpointCell = r;
        return r;
    }
    else {
        return m_checkpointCell;
    }
}

std::stack<Cell*> History::getCheckpointPath() {

    std::stack<Cell*> path;
    Cell* runner = getCheckpointCell();

    //std::cout << "Checkpoint Cell (" << runner->getX() << "," << runner->getY() << ")" << std::endl;

    // Note that the path should include the origin since we need to perform
    // updates at the origin in case it is our checkpoint cell
    while (runner != NULL) {
        path.push(runner);
        runner = runner->getPrev(); 
    }

    // print the path
    std::stack<Cell*> temp = path;
    while (!temp.empty()) {
        Cell* r = temp.top();
        temp.pop();
        std::cout << "Path step (" << r->getX() << "," << r->getY() << ")" << std::endl; // TODO
    }

    return path;
}

std::stack<Cell*> History::getCheckpointStack() {

    std::stack<Cell*> theStack;
    // Check to see if we've moved enough steps after an undo request was made to
    // advance the checkpoint. If so, update the checkpoint stack? and stack? // TODO
    // If not, simply return the last checkpoint stack.
    if (size() == m_stm) {
        m_checkpointStack = m_stacks.front();
        //return m_stacks.front();
        theStack = m_stacks.front();
    }
    else {
        //return m_checkpointStack;
        theStack = m_checkpointStack;
    }


    // Print the stack
    std::stack<Cell*> temp = theStack;
    int si = temp.size();
    for (int i = 0; i < si; i++) {
        Cell* c = temp.top();
        temp.pop();
        std::cout << "Checkpoint stack (" << c->getX() << "," << c->getY() << ")" << std::endl;
    }

    return theStack;
}

void History::moved(Cell* movedTo) {

    Cell* chek = m_checkpointCell;
    std::cout << "Checkpoint cell: (" << chek->getX() << "," << chek->getY() << ")" << std::endl;

    m_path.push(movedTo);
    std::cout << "Moved to cell (" << movedTo->getX() << "," << movedTo->getY() << ")" << std::endl;

    int srefCount = m_stackReferenceCounts.back();
    m_stackReferenceCounts.pop_back();
    m_stackReferenceCounts.push_back(srefCount+1);

    int crefCount = m_modifiedCellsReferenceCounts.back();
    m_modifiedCellsReferenceCounts.pop_back();
    m_modifiedCellsReferenceCounts.push_back(crefCount+1);

    // If the size of the path is larger than our short term memory (which it needn't
    // be) then reduce the reference counts and pop the appropriate number of things
    // off of the stacks.
    if (m_path.size() > m_stm + 1) {

        m_path.pop(); 

        int srefCount2 = m_stackReferenceCounts.front();
        m_stackReferenceCounts.pop_front();
        m_stackReferenceCounts.push_front(srefCount2-1);

        int crefCount2 = m_modifiedCellsReferenceCounts.front();
        m_modifiedCellsReferenceCounts.pop_front();
        m_modifiedCellsReferenceCounts.push_front(crefCount2-1);
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

    /*
    std::stack<Cell*> te = newStack;// TODO
    int size = te.size();
    for (int i = 0; i < size; i++) {
        Cell* c = te.top();
        te.pop();
        std::cout << "On stack (" << c->getX() << "," << c->getY() << ")" << std::endl;
    }
    std::cout << std::endl;
    */

    int temp = m_stackReferenceCounts.back();
    m_stackReferenceCounts.pop_back();
    m_stackReferenceCounts.push_back(temp-1);
    m_stackReferenceCounts.push_back(1);

    printS();// TODO
}

void History::modifiedCellsUpdate(std::list<std::pair<Cell*, int>> cells) {

    // Analogous to stackUpdate
    m_modifiedCells.push(cells);
    int temp = m_modifiedCellsReferenceCounts.back();
    m_modifiedCellsReferenceCounts.pop_back();
    m_modifiedCellsReferenceCounts.push_back(temp-1);
    m_modifiedCellsReferenceCounts.push_back(1);

    //printC();//TODO
}

void History::resetModifiedCells() {

    std::queue<std::list<std::pair<Cell*, int>>> temp = m_modifiedCells;

    while (!temp.empty()) {
        std::list<std::pair<Cell*, int>> cellList = temp.front();
        temp.pop();
        for (std::list<std::pair<Cell*, int>>::iterator it = cellList.begin(); it != cellList.end() ; ++it) {
            //std::cout << "Resetting (" << (std::get<0>(*it))->getX() << ","
            //<< (std::get<0>(*it))->getY() << ")" << std::endl;// TODO
            (std::get<0>(*it))->setPrev(NULL);

            // 4 indicates that we should reset all wall values
            if ((std::get<1>(*it)) == 4) {
                for (int i = 0; i < 4; i++){
                    (std::get<0>(*it))->setWallInspected(i, false);
                }
            }
            else{ // Otherwise we only need to reset the one wall that was seen
                  // from the then current cell
                (std::get<0>(*it))->setWallInspected(std::get<1>(*it), false);
            }
        }
    }

// TODO: note sureeeeeeeeeeeeeeeeeeee
    while (!m_path.empty()) {
        m_path.pop();
    }
    while (!m_stacks.empty()) {
        m_stacks.pop();
    }
    while (!m_stackReferenceCounts.empty()) {
        m_stackReferenceCounts.pop_front();
    }
    while (!m_modifiedCells.empty()) {
        m_modifiedCells.pop();
    }
    while (!m_modifiedCellsReferenceCounts.empty()) {
        m_modifiedCellsReferenceCounts.pop_front();
    }

    // Push the stack [(0,0)] since (0,0) or is *always* our first target
    std::stack<Cell*> ccc;
    ccc.push(m_checkpointCell);
    m_stacks.push(ccc);
    m_stackReferenceCounts.push_back(1);

    m_path.push(m_checkpointCell);

    // Push an empty list of modified cells as a dummy node
    std::list<std::pair<Cell*, int>> empty;
    m_modifiedCells.push(empty);
    m_modifiedCellsReferenceCounts.push_back(1);
// TODO: note sureeeeeeeeeeeeeeeeeeee
}


// Prints stacks and ref count
void History::printS() {

    int s = m_stacks.size(); // == m_stackReferenceCounts.size()

    std::queue<std::stack<Cell*>> ss = m_stacks;
    std::list<int> ii = m_stackReferenceCounts;

    std::cout << "================== PRINTING STACKS =====================" << std::endl;

    for (int i = 0; i < s; i++) {
        std::cout << "<" << ii.front() << "> of {";
        int z = ss.front().size();
        for (int j = 0; j < z; j++) {
            std::cout << " (" << ss.front().top()->getX() << ","
            <<  ss.front().top()->getY() << ")";
            ss.front().pop();
        }
        std::cout << " }" << std::endl;
        ss.pop();
        ii.pop_front();
    }
}

// Prints cells and ref count
void History::printC() {

    int s = m_modifiedCells.size(); // == m_modifiedCellsReferenceCounts.size()

    std::queue<std::list<std::pair<Cell*, int>>> ss = m_modifiedCells;
    std::list<int> ii = m_modifiedCellsReferenceCounts;

    std::cout << "================== PRINTING MODIFIED CELLS =====================" << std::endl;

    for (int i = 0; i < s; i++) {
        std::cout << "<" << ii.front() << "> of {";
        int z = ss.front().size();
        for (int j = 0; j < z; j++) {
            std::cout << " (" << (std::get<0>(ss.front().front()))->getX() << ","
            <<  (std::get<0>(ss.front().front()))->getY() << ")";
            ss.front().pop_front();
        }
        std::cout << " }" << std::endl;
        ss.pop();
        ii.pop_front();
    }
}

