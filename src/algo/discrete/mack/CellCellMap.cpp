#include "CellCellMap.h"

#include "Assert.h"

namespace mack {

CellCellMap::CellCellMap() {
}

bool CellCellMap::hasKey(Cell* key) const {
    return m_keys.contains(key);
}

Cell* CellCellMap::get(Cell* key) const {
    ASSERT(hasKey(key));
    return m_values.at(m_keys.indexOf(key));
}

void CellCellMap::put(Cell* key, Cell* value) {
    if (m_keys.contains(key)) {
        m_values.set(m_keys.indexOf(key), value);    
    }
    else {
        m_keys.append(key);
        m_values.append(value);
    }
}

} // namespace mack
