#include "Position.h"

namespace mackAlgoTwo {

unsigned char getX(unsigned char position) {
    // xxxxyyyy >> 4 = 0000xxxx
    return position >> 4;
}

unsigned char getY(unsigned char position) {
    // xxxxyyyy & 00001111 = 0000yyyy
    return position & 15;
}

unsigned char getPosition(unsigned char x, unsigned char y) {
    // 0000xxxx << 4 = xxxx0000
    // xxxx0000 | 0000yyyy = xxxxyyyy
    return x << 4 | y;
}

} // namespace mackAlgoTwo
