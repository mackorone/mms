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

    // TODO: MACK
    void heapify(int index);

private:
    int m_size;
    int m_capacity;
    Cell** m_data;

    void increaseCapacity();

    int getParentIndex(int index); 
    int getLeftChildIndex(int index); 
    int getRightChildIndex(int index); 

    Cell* getLeftChildOrNull(int index);
    Cell* getRightChildOrNull(int index);
    int getMinChildIndex(int index);

    // TODO: MACK - returns the new index in the heap
    void heapifyUp(int index);
    void heapifyDown(int index);
    void swap(int indexOne, int indexTwo);
};

} // namespace mackAlgoTwo
