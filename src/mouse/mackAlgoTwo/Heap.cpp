#include "Heap.h"

#include <iostream> // TODO: MACK

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
    // TODO: MACK - we're updating incorrectly here...
    std::cout << "---HEAP---" << std::endl;
    for (int i = 0; i < m_size; i += 1) {
        std::cout << std::string("Index: ") + std::to_string(m_data[i]) +
        std::string("  Heap Index: ") + std::to_string(Maze::info[m_data[i]].heapIndex) << std::endl;
    }
    std::cout << "---CUR---" << std::endl;
    std::cout << std::string("Index: ") + std::to_string(mazeIndex) + 
        std::string("  Heap Index: ") + std::to_string(Maze::info[mazeIndex].heapIndex) << std::endl;
    std::cout << std::endl;
    ASSERT_LE(0, Maze::info[mazeIndex].heapIndex);
    ASSERT_LE(Maze::info[mazeIndex].heapIndex, 63); // TODO: MACK
    heapifyUp(Maze::info[mazeIndex].heapIndex);
}

byte Heap::pop() {
    ASSERT_LT(0, m_size);
    byte mazeIndex = m_data[0];
    Maze::info[mazeIndex].heapIndex = 255; // TODO: MACK - hmmm
    m_data[0] = m_data[m_size - 1];
    Maze::info[m_data[0]].heapIndex = 0;
    m_size -= 1;
    if (1 < m_size) {
        heapifyDown(0);
    }
    return mazeIndex;
}

byte Heap::size() {
    return m_size;
}

void Heap::increaseCapacity() {
    m_capacity *= 2;
    byte* data = new byte[m_capacity];
    for (byte i = 0; i < m_size; i += 1) {
        data[i] = m_data[i];
    }
    delete[] m_data;
    m_data = data;
}

byte Heap::getParentIndex(byte heapIndex) {
    if (heapIndex == 0) {
        return 255;
    }
    return (heapIndex - 1) / 2;
}

byte Heap::getLeftChildIndex(byte heapIndex) {
    if (126 < heapIndex) {
        return 255;
    }
    return (heapIndex * 2) + 1;
}

byte Heap::getRightChildIndex(byte heapIndex) {
    if (126 < heapIndex) {
        return 255;
    }
    return (heapIndex + 1) * 2;
}

byte Heap::getLeftChildOrNull(byte heapIndex) {
    // TODO: MACK - fix these...
    return (
        getLeftChildIndex(heapIndex) < m_size ?
        m_data[getLeftChildIndex(heapIndex)] : 255
    );
}

byte Heap::getRightChildOrNull(byte heapIndex) {
    return (
        getRightChildIndex(heapIndex) < m_size ?
        m_data[getRightChildIndex(heapIndex)] : 255
    );
}

byte Heap::getMinChildIndex(byte heapIndex) {
    byte leftChildMazeIndex = getLeftChildOrNull(heapIndex);
    byte rightChildMazeIndex = getRightChildOrNull(heapIndex);

    byte left = getLeftChildIndex(heapIndex);
    byte right = getRightChildIndex(heapIndex);

    if (m_size <= left) {
        return 255;
    }
    if (m_size <= right) {
        return left;
    }

    return (
        Maze::info[m_data[left]].distance < Maze::info[m_data[right]].distance ?
        left : right
    );
}

void Heap::heapifyUp(byte heapIndex) {
    ASSERT_LE(0, heapIndex);
    ASSERT_LT(heapIndex, m_size);
    byte parentIndex = getParentIndex(heapIndex);
    while (
        255 != parentIndex &&
        Maze::info[m_data[heapIndex]].distance < Maze::info[m_data[parentIndex]].distance
    ) {
        swap(heapIndex, parentIndex);
        heapIndex = parentIndex;
        parentIndex = getParentIndex(heapIndex);
    }
}

void Heap::heapifyDown(byte heapIndex) {
    ASSERT_LE(0, heapIndex);
    ASSERT_LT(heapIndex, m_size);
    byte minChildIndex = getMinChildIndex(heapIndex);
    while (
        255 != minChildIndex &&
        Maze::info[m_data[minChildIndex]].distance < Maze::info[m_data[heapIndex]].distance
    ) {
        swap(heapIndex, minChildIndex);
        heapIndex = minChildIndex;
        minChildIndex = getMinChildIndex(heapIndex);
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
