#pragma once

namespace mack {

class Cell;

class CellQueue {

static const int MAX_CAPACITY = 256;

public:
    CellQueue();
    CellQueue(const CellQueue& other);
    void operator=(const CellQueue& other);
    ~CellQueue();

    bool empty() const;
    void enqueue(Cell* cell);
    Cell* dequeue();

    // Used for implementation only
    int _getHead() const;
    int _getTail() const;
    Cell** _getData() const;

private:
    int m_head;
    int m_tail;
    Cell** m_data;
};

} // namespace mack
