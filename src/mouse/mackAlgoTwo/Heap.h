#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

class Heap {

// TODO: MACK Restrict the capacity to 128?
static const byte DEFAULT_CAPACITY = 64;

public:
    Heap();
    ~Heap();

    bool empty();
    void push(byte mazeIndex);
    void update(byte mazeIndex);
    byte pop();

    // TODO: MACK
    byte size();

private:
    byte m_size;
    byte m_capacity;
    byte* m_data;

    void increaseCapacity();

    byte getParentIndex(byte heapIndex); 
    byte getLeftChildIndex(byte heapIndex); 
    byte getRightChildIndex(byte heapIndex); 
    byte getMinChildIndex(byte heapIndex);

    void heapifyUp(byte heapIndex);
    void heapifyDown(byte heapIndex);
    void swap(byte heapIndexOne, byte heapIndexTwo);
};

} // namespace mackAlgoTwo
