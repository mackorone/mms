#pragma once

#include "CellList.h"

namespace mack {

class CellCellMap {

public:
    CellCellMap();

    bool hasKey(Cell* key) const;
    Cell* get(Cell* key) const;
    void put(Cell* key, Cell* value);

private:
    CellList m_keys;
    CellList m_values;

};

} // namespace mack
