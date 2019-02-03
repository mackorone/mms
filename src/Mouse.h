#pragma once

#include <QMap>
#include <QMutex>
#include <QPair>
#include <QString>
#include <QVector>

#include "units/Coordinate.h"

#include "Direction.h"
#include "Maze.h"
#include "Polygon.h"

namespace mms {

class Mouse {

public:

    Mouse();

    // Resets the mouse to the beginning of the maze
    void reset();

    // Sets the current translation and rotation of the mouse
    void teleport(const Coordinate& translation, const Angle& rotation);

    // Gets the current discretized translation and rotation of the mouse
    QPair<int, int> getCurrentDiscretizedTranslation() const;
    Direction getCurrentDiscretizedRotation() const;

    // Retrieves the polygon of just the body of the mouse
    Polygon getCurrentBodyPolygon() const;
    Polygon getCurrentWheelPolygon() const;

private:

    // The translation and rotation of the mouse
    Coordinate m_initialTranslation;
    Coordinate m_currentTranslation;
    Angle m_initialRotation;
    Angle m_currentRotation;

    // The parts of the mouse at the starting location
    Polygon m_initialBodyPolygon;
    Polygon m_initialWheelPolygon;
    Polygon getCurrentPolygon(const Polygon& initialPolygon) const;
};

} 
