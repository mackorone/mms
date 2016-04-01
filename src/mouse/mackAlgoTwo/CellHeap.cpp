#include "CellHeap.h"

#include <cstdlib>

#include "Assert.h"
#include "Cell.h"

namespace mackAlgoTwo {

CellHeap::CellHeap() :
    m_size(0),
    m_capacity(DEFAULT_CAPACITY),
    m_data(new Cell*[DEFAULT_CAPACITY]) {
}

CellHeap::~CellHeap() {
    delete[] m_data;
}

bool CellHeap::empty() const {
    return m_size == 0;
}

void CellHeap::push(Cell* cell) {
    if (m_size == m_capacity) {
        increaseCapacity();
    }
    ASSERT_LT(m_size, m_capacity);
    m_data[m_size] = cell;
    m_size += 1;
    heapifyUp(m_size - 1);
}

Cell* CellHeap::pop() {
    ASSERT_LT(0, m_size);
    Cell* value = m_data[0];
    m_size -= 1;
    m_data[0] = m_data[m_size];
    heapifyDown(0);
    return value;
}

void CellHeap::increaseCapacity() {
    m_capacity *= 2;
    Cell** data = new Cell*[m_capacity];
    for (int i = 0; i < m_size; i += 1) {
        data[i] = m_data[i];
    }
    delete[] m_data;
    m_data = data;
}

int CellHeap::getParentIndex(int index) {
    return (index - 1) / 2;
}

int CellHeap::getLeftChildIndex(int index) {
    return (index * 2) + 1;
}

int CellHeap::getRightChildIndex(int index) {
    return (index + 1) * 2;
}

void CellHeap::swap(int indexOne, int indexTwo) {
    ASSERT_LE(0, indexOne);
    ASSERT_LE(0, indexTwo);
    ASSERT_LT(indexOne, m_size);
    ASSERT_LT(indexTwo, m_size);
    Cell* temp = m_data[indexOne];
    m_data[indexOne] = m_data[indexTwo];
    m_data[indexTwo] = temp;
}

void CellHeap::heapifyUp(int index) {
    while (
        0 <= getParentIndex(index) &&
        m_data[index]->getDistance() < m_data[getParentIndex(index)]->getDistance()
    ) {
        swap(index, getParentIndex(index));
        index = getParentIndex(index);
    }
}

void CellHeap::heapifyDown(int index) {
    Cell*  leftChild = getChild(index, true);
    Cell* rightChild = getChild(index, false);
    while (
        ( leftChild != NULL &&  leftChild->getDistance() < m_data[index]->getDistance()) ||
        (rightChild != NULL && rightChild->getDistance() < m_data[index]->getDistance())
    ) {
        int minChildIndex = getLeftChildIndex(index);
        if (
            rightChild != NULL &&
            rightChild->getDistance() < m_data[minChildIndex]->getDistance()
        ) {
            minChildIndex = getRightChildIndex(index);
        }
        swap(index, minChildIndex);
        index = minChildIndex;
        leftChild = getChild(index, true);
        rightChild = getChild(index, false);
    }
}

Cell* CellHeap::getChild(int index, bool left) {
    Cell* child = NULL;
    if (left) {
        if (getLeftChildIndex(index) < m_size) {
            child = m_data[getLeftChildIndex(index)];
        }
    }
    else {
        if (getRightChildIndex(index) < m_size) {
            child = m_data[getRightChildIndex(index)];
        }
    }
    return child;
}

} // namespace mackAlgoTwo
