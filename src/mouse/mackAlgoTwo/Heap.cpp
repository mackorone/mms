#include "Heap.h"

#include "Assert.h"
#include "Maze.h"

namespace mackAlgoTwo {

byte Heap::m_size = 0;
byte Heap::m_data[] = {0};

byte Heap::size() {
    return m_size;
}

void Heap::push(byte cell) {
    ASSERT_LT(m_size, CAPACITY);
    m_data[m_size] = cell;
    m_size += 1;
    if (1 < m_size) {
        heapifyUp(m_size - 1);
    }
}

void Heap::update(byte cell) {
    byte index = SENTINEL;
    for (byte i = 0; i < m_size; i += 1) {
        if (m_data[i] == cell) {
            index = i;
            break;
        }
    }
    ASSERT_NE(index, SENTINEL);
    heapifyUp(index);
}

byte Heap::pop() {
    ASSERT_LT(0, m_size);
    byte cell = m_data[0];
    m_data[0] = m_data[m_size - 1];
    m_size -= 1;
    if (1 < m_size) {
        heapifyDown(0);
    }
    return cell;
}

void Heap::clear() {
    m_size = 0;
}

byte Heap::getParentIndex(byte index) {
    if (index == 0) {
        return SENTINEL;
    }
    return (index - 1) / 2;
}

byte Heap::getLeftChildIndex(byte index) {
    if (getParentIndex(CAPACITY - 1) < index) {
        return SENTINEL;
    }
    return (index * 2) + 1;
}

byte Heap::getRightChildIndex(byte index) {
    if (getParentIndex(CAPACITY - 1) < index) {
        return SENTINEL;
    }
    return (index + 1) * 2;
}

byte Heap::getMinChildIndex(byte index) {
    byte left = getLeftChildIndex(index);
    byte right = getRightChildIndex(index);
    if (m_size <= left) {
        return SENTINEL;
    }
    if (m_size <= right) {
        return left;
    }
    return (
        Maze::getDistance(m_data[left]) < Maze::getDistance(m_data[right]) ?
        left : right
    );
}

void Heap::heapifyUp(byte index) {
    ASSERT_LE(0, index);
    ASSERT_LT(index, m_size);
    byte parentIndex = getParentIndex(index);
    while (
        parentIndex != SENTINEL &&
        Maze::getDistance(m_data[index]) < Maze::getDistance(m_data[parentIndex])
    ) {
        swap(index, parentIndex);
        index = parentIndex;
        parentIndex = getParentIndex(index);
    }
}

void Heap::heapifyDown(byte index) {
    ASSERT_LE(0, index);
    ASSERT_LT(index, m_size);
    byte minChildIndex = getMinChildIndex(index);
    while (
        minChildIndex != SENTINEL &&
        Maze::getDistance(m_data[minChildIndex]) < Maze::getDistance(m_data[index])
    ) {
        swap(index, minChildIndex);
        index = minChildIndex;
        minChildIndex = getMinChildIndex(index);
    }
}

void Heap::swap(byte indexOne, byte indexTwo) {
    ASSERT_LE(0, indexOne);
    ASSERT_LE(0, indexTwo);
    ASSERT_LT(indexOne, m_size);
    ASSERT_LT(indexTwo, m_size);
    ASSERT_NE(indexOne, indexTwo);
    byte temp = m_data[indexOne];
    m_data[indexOne] = m_data[indexTwo];
    m_data[indexTwo] = temp;
}

} // namespace mackAlgoTwo
