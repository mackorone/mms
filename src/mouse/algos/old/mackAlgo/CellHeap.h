#pragma once

namespace mackAlgo {

class Cell;

class CellHeap {

static const int DEFAULT_CAPACITY = 16;

public:
    CellHeap();
    CellHeap(const CellHeap& other);
    void operator=(const CellHeap& other);
    ~CellHeap();

    int size() const;
    int capacity() const;
    Cell* get(int index) const;

    Cell* pop();
    void push(Cell* cell);

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
};

} // namespace mackAlgo
