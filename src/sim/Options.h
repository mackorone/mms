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

// Each of the following is a subset of the StaticMouseAlgorithmOptions,
// and contains what is needed by that particular class

struct WorldOptions {
    std::string interfaceType;
};

struct ViewOptions {
    std::string interfaceType;
    int tileTextNumberOfRows;
    int tileTextNumberOfCols;
};

struct HeaderOptions {
    std::string mouseFile;
    std::string interfaceType;
    std::string initialDirection;
    int tileTextNumberOfRows;
    int tileTextNumberOfCols;
    double wheelSpeedFraction;
};

struct MouseInterfaceOptions {
    std::string interfaceType;
    int tileTextNumberOfRows;
    int tileTextNumberOfCols;
    double wheelSpeedFraction;
};

} // namespace sim
