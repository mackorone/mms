#pragma once

namespace sim {

struct StaticMouseAlgorithmOptions {
    QString mouseFile;
    QString interfaceType;
    QString initialDirection;
    int tileTextNumberOfRows;
    int tileTextNumberOfCols;
    double wheelSpeedFraction;
};

} // namespace sim
