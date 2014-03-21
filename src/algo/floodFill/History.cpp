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

    // The first thing that the floodfill algo will do after this initialization
    // is update the origins values, and then call stackUpdate() and
    // modifiedCellsUpdate(). However, we need to "move to" the origin since
    // the Floodfill code will not do this for us. Here, we manually perform
    // the necessary operations of the moved() method since the initial conditions
    // break some of the invariant properties of History, namely that the path size
    // is never 0.
    m_path.push(origin);
    //m_steps = 0;

    // Note however, that we don't automatically add anything to m_modifiedCells since
    // the only thing that the origin won't do is append to the path. It will still
    // append the new stack and modified cells.
}

int History::size() {
    // The number of steps taken by the robot since reaching the checkpoint
    return m_path.size() - 1;
}
// TODO: Consolidate checkpoint cell and stack into a single pair
Cell* History::getCheckpointCell() {

    //std::cout << "Getting checkpoint cell" << std::endl;

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

    std::cout << "Checkpoint Cell (" << runner->getX() << "," << runner->getY() << ")" << std::endl;

    while (runner != NULL) {
        path.push(runner);
        if (runner->getPrev() == NULL) {
            std::cout << "RUNNER END Cell (" << runner->getX() << "," << runner->getY() << ")" << std::endl;
        }
        runner = runner->getPrev(); 
    }
    
    

    // The path always includes the origin, and since we can't and don't want 
    // to try to move to the origin, we can *always* pop it off safely
    path.pop();

    // print the path
    std::stack<Cell*> temp = path;
    while (!temp.empty()) {
        Cell* r = temp.top();
        temp.pop();
        if (temp.size() > 0) {
            std::cout << "Path step (" << r->getX() << "," << r->getY() << ")" << std::endl; // TODO
        }
    }
    std::cout << "Ending at checkpoint (" << getCheckpointCell()->getX() << ","
    << getCheckpointCell()->getY() << ")" << std::endl;

    //std::cout << "Finished obtaining checkpoint path" << std::endl;
    return path;
}

std::stack<Cell*> History::getCheckpointStack() {

    //std::cout << "Getting checkpoint stack" << std::endl;

    // Print the stack
    /*
    std::stack<Cell*> temp = m_checkpointStack;
    while (!temp.empty()) {
        Cell* c = temp.top();
        temp.pop();
        std::cout << "On cp stack (" << c->getX() << "," << c->getY() << ")" << std::endl;
    }
    */

    return m_checkpointStack;
}

void History::moved(Cell* movedTo) {

    m_path.push(movedTo);
    //std::cout << "Moved to cell (" << movedTo->getX() << "," << movedTo->getY() << ")" << std::endl;
    //m_steps++;

    // Assertion - Neither m_stacks nor m_stackReferenceCounts should ever be empty
    if (m_stackReferenceCounts.empty() || m_stacks.empty()) {
        std::cout << "Error - History object has zero stack references" << std::endl;
        exit(0);
    }
    int srefCount = m_stackReferenceCounts.back();
    m_stackReferenceCounts.pop_back();
    m_stackReferenceCounts.push_back(srefCount+1);

    // Every move we push an empty list onto the list of lists of modified cells
    std::list<std::pair<Cell*, int>> empty;
    m_modifiedCells.push_back(empty);

    // If the size of the path is larger than our short term memory (which it needn't
    // be) then reduce the reference counts and pop the appropriate number of things
    // off of the stacks.
    if (size() > m_stm) {

        m_path.pop();

        int srefCount2 = m_stackReferenceCounts.front();
        m_stackReferenceCounts.pop_front();
        m_stackReferenceCounts.push_front(srefCount2-1);
    }
    if (m_stackReferenceCounts.front() == 0) {
        m_stackReferenceCounts.pop_front();
        m_stacks.pop();
        m_checkpointStack = m_stacks.front(); // TODO: Does this work?
        if (m_stacks.front().top()->getPrev() != NULL) {
            m_checkpointCell = m_stacks.front().top()->getPrev(); // TODO: Does this work?
        }
        else {
            std::cout << "Error - checkpoint has not prev Cell" << std::endl;
            exit(0);
        }
    }

    // Also make sure to only keep the correct number of modified cells
    if (m_modifiedCells.size() > m_stm) {
        m_modifiedCells.pop_front();
    }
}

void History::stackUpdate(std::stack<Cell*> newStack) {

    //std::cout << "Attempting stack update" << std::endl;

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

    //printS();// TODO
}

void History::modifiedCellsUpdate(std::list<std::pair<Cell*, int>> cells) {

    // The only situation when this list will be empty is immediately after
    // returning to the origin after undo is called. This is because the checkpoint
    // stack will contain the origin, and thus won't move from the origin to get to
    // the target (the origin) but WILL perform the appropriate updates.
    if (m_modifiedCells.size() > 0) {
        m_modifiedCells.pop_back();
    }
    m_modifiedCells.push_back(cells);

    //printC();//TODO
}

void History::resetModifiedCells() {

    std::list<std::list<std::pair<Cell*, int>>> temp = m_modifiedCells;

    // TODO: Go in reverse, set the prev values
    while (!temp.empty()) {
        std::list<std::pair<Cell*, int>> cellList = temp.front();
        temp.pop_front();
        for (std::list<std::pair<Cell*, int>>::iterator it = cellList.begin(); it != cellList.end() ; ++it) {

            //std::cout << "Resetting (" << (std::get<0>(*it))->getX() << ","
            //<< (std::get<0>(*it))->getY() << ")" << std::endl;// TODO

            // 4 indicates that the cell was actually traversed
            if ((std::get<1>(*it)) == 4) {
                for (int i = 0; i < 4; i++){

                    // Set the wall to false as well, because we don't check the wall inspected
                    // values explicitely when we're considering adding something to the stack
                    (std::get<0>(*it))->setWall(i, false);
                    (std::get<0>(*it))->setWallInspected(i, false);
                }

                // A value of 4 indicates that this cell was actually traversed (AKA a target). Since
                // it was a target, it means it had to have been on the stack, and thus will be put
                // back on the stack after it is rediscovered. 
                (std::get<0>(*it))->setPrev(NULL);

            }
            else{ // Otherwise we only need to reset the one wall that was seen
                  // from the then current cell
                (std::get<0>(*it))->setWall(std::get<1>(*it), false);
                (std::get<0>(*it))->setWallInspected(std::get<1>(*it), false);
            }
        }
    }

    // After we get a new checkpoint, we empty everything // TODO: Not sure about this yet
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
        m_modifiedCells.pop_front();
    }

    m_stacks.push(m_checkpointStack);
    m_stackReferenceCounts.push_back(1);
    m_path.push(getCheckpointCell());
    //m_steps = 0;
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

    std::list<std::list<std::pair<Cell*, int>>> ss = m_modifiedCells;

    std::cout << "================== PRINTING MODIFIED CELLS =====================" << std::endl;

    while (!ss.empty()) {
        std::cout << "{";
        while (!ss.front().empty()) {
            std::cout << " (" << (std::get<0>(ss.front().front()))->getX() << ","
            <<  (std::get<0>(ss.front().front()))->getY() << ")";
            ss.front().pop_front();
        }
        std::cout << " }" << std::endl;
        ss.pop_front();
    }
}

