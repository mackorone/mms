#include "CellList.h"

#include "Assert.h"

namespace mack {

CellList::CellList() : m_size(0), m_capacity(DEFAULT_CAPACITY) {
    m_data = new Cell*[m_capacity];
}

CellList::CellList(const CellList& other) {
    CellList();
    *this = other;
}

void CellList::operator=(const CellList& other) {

    // Increase the capacity if necessary
    while (m_capacity < other.capacity()) {
        increaseCapacity();
    }

    // Add the items and increase the size
    for (int i = 0; i < other.size(); i += 1) {
        m_data[i] = other.get(i);
    }
    m_size = other.size();
}

CellList::~CellList() {
    delete[] m_data;
}

int CellList::size() const {
    return m_size;
}

int CellList::capacity() const {
    return m_capacity;
}

bool CellList::contains(Cell* cell) const {
    for (int i = 0; i < m_size; i += 1) {
        if (m_data[i] == cell) {
            return true;
        }
    }
    return false;
}

int CellList::indexOf(Cell* cell) const {
    ASSERT(contains(cell));
    int index = 0;
    while (get(index) != cell) {
        index += 1;
    }
    return index;
}

Cell* CellList::get(int index) const {
    ASSERT(0 <= index && index < size());
    return m_data[index];
}

void CellList::append(Cell* cell) {
    if (m_size == m_capacity) {
        increaseCapacity();
    }
    m_data[m_size] = cell;
    m_size += 1;
}

void CellList::set(int index, Cell* cell) {
    ASSERT(0 <= index && index < size());
    m_data[index] = cell;
}

void CellList::increaseCapacity() {
    m_capacity *= 2;
    Cell** data = new Cell*[m_capacity];
    for (int i = 0; i < m_size; i += 1) {
        data[i] = m_data[i];
    }
    delete[] m_data;
    m_data = data;
}

} // namespace mack
