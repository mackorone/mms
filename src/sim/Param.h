#pragma once

#include <QString>

namespace mms {

// Wrapper for the static call to Param::getInstance()
class Param;
Param* P();

class Param {

public:

    // Returns a pointer to the singleton parameter object
    static Param* getInstance();

    // Simulation parameters
    int randomSeed();

    // Maze parameters
    double wallWidth();
    double wallLength();
    bool mazeMirrored(); // TODO: MACK
    int mazeRotations(); // TODO: MACK

private:

    // A private constructor is used to ensure only one instance of this class exists
    Param();

    // A pointer to the actual instance of the class
    static Param* INSTANCE;

    // Members
    int m_randomSeed;
    double m_wallWidth;
    double m_wallLength;
    bool m_mazeMirrored;
    int m_mazeRotations;
};

} // namespace mms
