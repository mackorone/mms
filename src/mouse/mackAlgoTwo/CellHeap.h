#pragma once

namespace mackAlgoTwo {

class Cell;

class CellHeap {

static const int DEFAULT_CAPACITY = 16;

public:
    CellHeap();
    ~CellHeap();

    bool empty() const;
    void push(Cell* cell);
    Cell* pop();

private:
    int m_size;
    int m_capacity;
    Cell** m_data;

    void increaseCapacity();

    int getParentIndex(int index); 
    int getLeftChildIndex(int index); 
    int getRightChildIndex(int index); 

    void swap(int indexOne, int indexTwo);
    void heapifyUp(int index);
    void heapifyDown(int index);
    Cell* getChild(int index, bool left);
};

} // namespace mackAlgoTwo
