#pragma once

namespace mackAlgoTwo {

class Cell;

struct Info {
    int sequenceNumber;
    Cell* parent;
    int sourceDirection;
    int straightAwayLength;
    float distance;
    int heapIndex;
    /* // TODO: MACK
    unsigned char sequenceNumber;
    float         distance;
    Cell*         parent;
    unsigned char sourceDirection; // 2 bits
    unsigned char straightAwayLength; // 4 bits
    unsigned char heapIndex;
    */
};

} // namespace mackAlgoTwo
