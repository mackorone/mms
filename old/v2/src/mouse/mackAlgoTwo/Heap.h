#pragma once

#include "Byte.h"

namespace mackAlgoTwo {

class Heap {

public:

    static byte size();
    static void push(byte cell);
    static void update(byte cell);
    static byte pop();
    static void clear();

private:

    static const byte CAPACITY = 127;
    static const byte SENTINEL = 255;

    static byte m_size;
    static byte m_data[CAPACITY];

    static byte getParentIndex(byte index); 
    static byte getLeftChildIndex(byte index); 
    static byte getRightChildIndex(byte index); 
    static byte getMinChildIndex(byte index);

    static void heapifyUp(byte index);
    static void heapifyDown(byte index);
    static void swap(byte indexOne, byte indexTwo);
};

} // namespace mackAlgoTwo
