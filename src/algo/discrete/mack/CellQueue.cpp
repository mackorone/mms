#include "CellQueue.h"

#include "Assert.h"

namespace mack {

CellQueue::CellQueue() : m_head(0), m_tail(0), m_data(new Cell*[MAX_CAPACITY]) {
}

CellQueue::CellQueue(const CellQueue& other) {
    CellQueue();
    *this = other;
}

void CellQueue::operator=(const CellQueue& other) {
    int head = other._getHead();
    int tail = other._getTail();
    while (head != tail) {
        enqueue(_getData()[head]);
        head += 1;
    }
}

CellQueue::~CellQueue() {
    delete[] m_data;
}

bool CellQueue::empty() const {
    return m_head == m_tail;
}

void CellQueue::enqueue(Cell* cell) {
    m_data[m_tail] = cell;
    m_tail += (1 + MAX_CAPACITY) % MAX_CAPACITY;
}

Cell* CellQueue::dequeue() {
    ASSERT(!empty());
    Cell* data = m_data[m_head];
    m_data += (1 + MAX_CAPACITY) % MAX_CAPACITY;
    return data;
}

int CellQueue::_getHead() const {
    return m_head;
}

int CellQueue::_getTail() const {
    return m_tail;
}

Cell** CellQueue::_getData() const {
    return m_data;
}

} // namespace mack
