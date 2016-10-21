#include "History.h"

#include "Assert.h"

byte History::m_size = 0;
byte History::m_tail = 0;
bool History::m_infoAdded = false;
twobyte History::m_data[] = {0};

byte History::size() {
    return m_size;
}

void History::add(byte cell, byte data) {
    m_data[m_tail] = cell << 8 | data;
    m_infoAdded = true;
}

void History::move() {
    if (!m_infoAdded) {
        m_data[m_tail] = 0;
    }
    m_infoAdded = false;
    m_tail = (m_tail + 1) % CAPACITY;
    if (m_size < CAPACITY) {
        m_size += 1; 
    }
}

twobyte History::pop() {
    ASSERT_LT(0, m_size);
    m_tail = (m_tail - 1 + CAPACITY) % CAPACITY;
    twobyte cellAndData = m_data[m_tail];
    m_data[m_tail] = 0;
    m_size -= 1;
    return cellAndData;
}

byte History::cell(twobyte cellAndData) {
    return cellAndData >> 8;
}

byte History::data(twobyte cellAndData) {
    return cellAndData & 255;
}
