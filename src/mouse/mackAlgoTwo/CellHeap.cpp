#include "CellHeap.h"

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
    m_data[m_size]->info.heapIndex = m_size;
    m_size += 1;
    if (1 < m_size) {
        heapifyUp(m_size - 1);
    }
}

void CellHeap::update(Cell* cell) {
    ASSERT_LE(0, cell->info.heapIndex);
    heapifyUp(cell->info.heapIndex);
}

Cell* CellHeap::pop() {
    ASSERT_LT(0, m_size);
    Cell* cell = m_data[0];
    cell->info.heapIndex = -1;
    m_data[0] = m_data[m_size - 1];
    m_data[0]->info.heapIndex = 0;
    m_size -= 1;
    if (1 < m_size) {
        heapifyDown(0);
    }
    return cell;
}

int CellHeap::size() const {
    return m_size;
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
    if (index < 0) {
        return -1;
    }
    return (index - 1) / 2;
}

int CellHeap::getLeftChildIndex(int index) {
    return (index * 2) + 1;
}

int CellHeap::getRightChildIndex(int index) {
    return (index + 1) * 2;
}

Cell* CellHeap::getLeftChildOrNull(int index) {
    return (
        getLeftChildIndex(index) < m_size ?
        m_data[getLeftChildIndex(index)] : NULL
    );
}

Cell* CellHeap::getRightChildOrNull(int index) {
    return (
        getRightChildIndex(index) < m_size ?
        m_data[getRightChildIndex(index)] : NULL
    );
}

int CellHeap::getMinChildIndex(int index) {
    Cell* leftChild = getLeftChildOrNull(index);
    Cell* rightChild = getRightChildOrNull(index);
    if (leftChild == NULL) {
        return -1;
    }
    if (rightChild == NULL) {
        return getLeftChildIndex(index);
    } 
    return (
        leftChild->info.distance < rightChild->info.distance ?
        getLeftChildIndex(index) : getRightChildIndex(index)
    );
}

void CellHeap::heapifyUp(int index) {
    ASSERT_LE(0, index);
    ASSERT_LT(index, m_size);
    int parentIndex = getParentIndex(index);
    while (
        0 <= parentIndex &&
        m_data[index]->info.distance < m_data[parentIndex]->info.distance
    ) {
        swap(index, parentIndex);
        index = parentIndex;
        parentIndex = getParentIndex(index);
    }
}

void CellHeap::heapifyDown(int index) {
    ASSERT_LE(0, index);
    ASSERT_LT(index, m_size);
    int minChildIndex = getMinChildIndex(index);
    while (
        0 <= minChildIndex &&
        m_data[minChildIndex]->info.distance < m_data[index]->info.distance
    ) {
        swap(index, minChildIndex);
        index = minChildIndex;
        minChildIndex = getMinChildIndex(index);
    }
}

void CellHeap::swap(int indexOne, int indexTwo) {
    ASSERT_LE(0, indexOne);
    ASSERT_LE(0, indexTwo);
    ASSERT_LT(indexOne, m_size);
    ASSERT_LT(indexTwo, m_size);
    Cell* temp = m_data[indexOne];
    m_data[indexOne] = m_data[indexTwo];
    m_data[indexTwo] = temp;
    m_data[indexOne]->info.heapIndex = indexOne;
    m_data[indexTwo]->info.heapIndex = indexTwo;
}

} // namespace mackAlgoTwo
