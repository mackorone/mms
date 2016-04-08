#include "Heap.h"

#include "Assert.h"
#include "Maze.h"

namespace mackAlgoTwo {

Heap::Heap() :
    m_size(0),
    m_capacity(DEFAULT_CAPACITY),
    m_data(new byte[DEFAULT_CAPACITY]) {
}

Heap::~Heap() {
    delete[] m_data;
}

byte Heap::size() {
    return m_size;
}

bool Heap::empty() {
    return m_size == 0;
}

void Heap::push(byte mazeIndex) {
    if (m_size == m_capacity) {
        increaseCapacity();
    }
    ASSERT_LT(m_size, m_capacity);
    m_data[m_size] = mazeIndex;
    Maze::info[mazeIndex].heapIndex = m_size;
    m_size += 1;
    if (1 < m_size) {
        heapifyUp(m_size - 1);
    }
}

void Heap::update(byte mazeIndex) {
    ASSERT_LE(0, Maze::info[mazeIndex].heapIndex);
    heapifyUp(Maze::info[mazeIndex].heapIndex);
}

byte Heap::pop() {
    ASSERT_LT(0, m_size);
    byte mazeIndex = m_data[0];
    Maze::info[mazeIndex].heapIndex = SENTINEL;
    m_data[0] = m_data[m_size - 1];
    Maze::info[m_data[0]].heapIndex = 0;
    m_size -= 1;
    if (1 < m_size) {
        heapifyDown(0);
    }
    return mazeIndex;
}

void Heap::increaseCapacity() {

    // The possible capacity values should be as follows:
    // 0 -> 1 -> 3 -> 7 -> 15 -> 31 -> 63 -> 127 -> 255
    m_capacity += m_capacity + 1;
    ASSERT_LE(m_capacity, MAXIMUM_CAPACITY);

    // Create a new data array, copy the
    // data over, and delete the old one
    byte* data = new byte[m_capacity];
    for (byte i = 0; i < m_size; i += 1) {
        data[i] = m_data[i];
    }
    delete[] m_data;
    m_data = data;
}

byte Heap::getParentIndex(byte heapIndex) {
    if (heapIndex == 0) {
        return SENTINEL;
    }
    return (heapIndex - 1) / 2;
}

byte Heap::getLeftChildIndex(byte heapIndex) {
    // If the heap index is larger than the largest
    // possible parent index, just return the SENTINEL
    if (getParentIndex(MAXIMUM_CAPACITY - 1) < heapIndex) {
        return SENTINEL;
    }
    return (heapIndex * 2) + 1;
}

byte Heap::getRightChildIndex(byte heapIndex) {
    // If the heap index is larger than the largest
    // possible parent index, just return the SENTINEL
    if (getParentIndex(MAXIMUM_CAPACITY - 1) < heapIndex) {
        return SENTINEL;
    }
    return (heapIndex + 1) * 2;
}

byte Heap::getMinChildIndex(byte heapIndex) {

    byte left = getLeftChildIndex(heapIndex);
    byte right = getRightChildIndex(heapIndex);

    if (m_size <= left) {
        return SENTINEL;
    }

    if (m_size <= right) {
        return left;
    }

    return (Maze::info[m_data[left]].distance < Maze::info[m_data[right]].distance ?  left : right);
}

void Heap::heapifyUp(byte heapIndex) {
    ASSERT_LE(0, heapIndex);
    ASSERT_LT(heapIndex, m_size);
    byte parentHeapIndex = getParentIndex(heapIndex);
    while (
        parentHeapIndex != SENTINEL &&
        Maze::info[m_data[heapIndex]].distance < Maze::info[m_data[parentHeapIndex]].distance
    ) {
        swap(heapIndex, parentHeapIndex);
        heapIndex = parentHeapIndex;
        parentHeapIndex = getParentIndex(heapIndex);
    }
}

void Heap::heapifyDown(byte heapIndex) {
    ASSERT_LE(0, heapIndex);
    ASSERT_LT(heapIndex, m_size);
    byte minChildHeapIndex = getMinChildIndex(heapIndex);
    while (
        minChildHeapIndex != SENTINEL &&
        Maze::info[m_data[minChildHeapIndex]].distance < Maze::info[m_data[heapIndex]].distance
    ) {
        swap(heapIndex, minChildHeapIndex);
        heapIndex = minChildHeapIndex;
        minChildHeapIndex = getMinChildIndex(heapIndex);
    }
}

void Heap::swap(byte heapIndexOne, byte heapIndexTwo) {
    ASSERT_LE(0, heapIndexOne);
    ASSERT_LE(0, heapIndexTwo);
    ASSERT_LT(heapIndexOne, m_size);
    ASSERT_LT(heapIndexTwo, m_size);
    ASSERT_NE(heapIndexOne, heapIndexTwo);
    byte temp = m_data[heapIndexOne];
    m_data[heapIndexOne] = m_data[heapIndexTwo];
    m_data[heapIndexTwo] = temp;
    Maze::info[m_data[heapIndexOne]].heapIndex = heapIndexOne;
    Maze::info[m_data[heapIndexTwo]].heapIndex = heapIndexTwo;
}

} // namespace mackAlgoTwo
