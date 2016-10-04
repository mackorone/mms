#pragma once

namespace mms {

struct StaticMouseAlgorithmOptions {
    // Fields and default values
    QString mouseFile = "default.xml";
    QString interfaceType = "DISCRETE";
    QString initialDirection = "NORTH";
    int tileTextNumberOfRows = 2;
    int tileTextNumberOfCols = 3;
    double wheelSpeedFraction = 1.0;
};

} // namespace mms
