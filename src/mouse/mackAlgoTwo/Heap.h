#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

// This class provides a min heap, where items (maze indicies) are
// heapified based on the cell distance, as calculated by Dijkstra's.
class Heap {

public:

    Heap();
    ~Heap();

    byte size();
    bool empty();
    void push(byte mazeIndex);
    void update(byte mazeIndex);
    byte pop();

private:

    // We should rarely exceed the default capacity, which is six levels deep.
    // However, if necessary, we can increase capacity up to eight levels deep.
    static const byte DEFAULT_CAPACITY = 63;
    static const byte MAXIMUM_CAPACITY = 255;

    // A maximum capacity of 255 means that the max index in the m_data array
    // will be 254. This leaves 255 available as a sentinel value, which is
    // nothing more than a way to represent NULL in byte form.
    static const byte SENTINEL = MAXIMUM_CAPACITY;

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
