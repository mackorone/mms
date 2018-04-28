#include "Param.h"

#include "ParamParser.h"

namespace mms {

Param* P() {
    return Param::getInstance();
}

Param* Param::INSTANCE = nullptr;
Param* Param::getInstance() {
    if (nullptr == INSTANCE) {
        INSTANCE = new Param();
    }
    return INSTANCE;
}

Param::Param() {
    m_randomSeed = 0; // TODO: MACK - remove this
    m_wallWidth = ParamParser::getDoubleIfHasDoubleAndInRange(
        "wall-width", 0.012, 0.006, 0.024);
    m_wallLength = ParamParser::getDoubleIfHasDoubleAndInRange(
        "wall-length", 0.168, 0.084, 0.336);
    m_mazeMirrored = ParamParser::getBoolIfHasBool(
        "maze-mirrored", false);
    m_mazeRotations = ParamParser::getIntIfHasIntAndInRange(
        "maze-rotations", 0, 0, 3);
}

int Param::randomSeed() {
    return m_randomSeed;
}

double Param::wallWidth() {
    return m_wallWidth;
}

double Param::wallLength() {
    return m_wallLength;
}

bool Param::mazeMirrored() {
    return m_mazeMirrored;
}

int Param::mazeRotations() {
    return m_mazeRotations;
}

} // namespace mms
