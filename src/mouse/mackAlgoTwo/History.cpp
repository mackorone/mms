#include "History.h"

namespace mackAlgoTwo {

byte History::m_size = 0;
byte History::m_tail = 0;
twobyte History::m_data[] = {0};

void History::movedTo(byte cell, byte data) {
    m_data[m_tail] = cell << 8 | data;
    m_tail = (m_tail + 1) % CAPACITY;
    if (m_size < CAPACITY) {
        m_size += 1; 
    }
}

void History::rollBack() {
    while (0 < m_size) {
        m_tail = (m_tail - 1 + CAPACITY) % CAPACITY;
        // TODO: MACK - do something with m_data[m_tail]
        m_size -= 1;
    }
}

} // namespace mackAlgoTwo
