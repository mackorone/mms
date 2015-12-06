#include "CellHeap.h"

#include <cstdlib>

#include "Assert.h"
#include "Cell.h"

namespace mackAlgo {

CellHeap::CellHeap() : m_size(0), m_capacity(DEFAULT_CAPACITY), m_data(new Cell*[DEFAULT_CAPACITY]) {
}

CellHeap::CellHeap(const CellHeap& other) : m_size(0), m_capacity(DEFAULT_CAPACITY), m_data(new Cell*[DEFAULT_CAPACITY]) {
    *this = other;
}

void CellHeap::operator=(const CellHeap& other) {
    while (m_capacity < other.capacity()) {
        increaseCapacity();
    }
    for (int i = 0; i < other.size(); i += 1) {
        m_data[i] = other.get(i);
    }
    m_size = other.size();
}

CellHeap::~CellHeap() {
    delete[] m_data;
}

int CellHeap::size() const {
    return m_size;
}

int CellHeap::capacity() const {
    return m_capacity;
}

Cell* CellHeap::get(int index) const {
    ASSERT(0 <= index && index < size());
    return m_data[index];
}

void CellHeap::push(Cell* cell) {
    if (m_size == m_capacity) {
        increaseCapacity();
    }
    m_data[m_size] = cell;
    m_size += 1;
    heapifyUp(m_size - 1);
}

Cell* CellHeap::pop() {
    ASSERT(m_size > 0);
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
    ASSERT(0 <= indexOne && indexOne < size());
    ASSERT(0 <= indexTwo && indexTwo < size());
    Cell* temp = m_data[indexOne];
    m_data[indexOne] = m_data[indexTwo];
    m_data[indexTwo] = temp;
}

void CellHeap::heapifyUp(int index) {
    while (0 <= getParentIndex(index) && m_data[index]->getDistance() < m_data[getParentIndex(index)]->getDistance()) {
        swap(index, getParentIndex(index));
        index = getParentIndex(index);
    }
}

void CellHeap::heapifyDown(int index) {

    Cell* leftChild = (getLeftChildIndex(index) < m_size ? m_data[getLeftChildIndex(index)] : NULL);
    Cell* rightChild = (getRightChildIndex(index) < m_size ? m_data[getRightChildIndex(index)] : NULL);

    while ((leftChild != NULL && leftChild->getDistance() < m_data[index]->getDistance())
        || (rightChild != NULL && rightChild->getDistance() < m_data[index]->getDistance())) {

        int minChildIndex = getLeftChildIndex(index);
        if (rightChild != NULL && rightChild->getDistance() < m_data[minChildIndex]->getDistance()) {
            minChildIndex = getRightChildIndex(index);
        }

        swap(index, minChildIndex);
        index = minChildIndex;
        leftChild = (getLeftChildIndex(index) < m_size ? m_data[getLeftChildIndex(index)] : NULL);
        rightChild = (getRightChildIndex(index) < m_size ? m_data[getRightChildIndex(index)] : NULL);
    }
}

} // namespace mackAlgo
