#pragma once

namespace mack {

class Cell;

class CellList {

static const int DEFAULT_CAPACITY = 4;

public:
    CellList();
    CellList(int capacity);
    CellList(const CellList& other);
    void operator=(const CellList& other);
    ~CellList();

    int size() const;
    int capacity() const;

    Cell* get(int index) const;
    void set(int index, Cell* cell);

    bool contains(Cell* cell) const;
    int indexOf(Cell* cell) const;

    void append(Cell* cell);

private:
    int m_size;
    int m_capacity;
    Cell** m_data;

    void increaseCapacity();
};

} // namespace mack
