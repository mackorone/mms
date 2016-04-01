#pragma once

namespace sim {

struct StaticMouseAlgorithmOptions {
    std::string mouseFile;
    std::string interfaceType;
    std::string initialDirection;
    int tileTextNumberOfRows;
    int tileTextNumberOfCols;
    double wheelSpeedFraction;
};

} // namespace sim
