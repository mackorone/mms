#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

// This class provides a min heap, where items (maze indicies) are
// heapified based on the cell distance, as calculated by Dijkstra's.
class Heap {

public:

    Heap();

    byte size();
    void push(byte cell);
    void update(byte cell);
    byte pop();

private:

    static const byte CAPACITY = 127;
    static const byte SENTINEL = 255;

    byte m_size;
    byte m_data[CAPACITY];

    byte getParentIndex(byte index); 
    byte getLeftChildIndex(byte index); 
    byte getRightChildIndex(byte index); 
    byte getMinChildIndex(byte index);

    void heapifyUp(byte index);
    void heapifyDown(byte index);
    void swap(byte index1, byte index2);
};

} // namespace mackAlgoTwo
