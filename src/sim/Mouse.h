#pragma once

#include <QMap>
#include <QMutex>
#include <QPair>
#include <QString>
#include <QVector>

#include "units/Coordinate.h"
#include "units/Duration.h"

#include "Direction.h"
#include "Maze.h"
#include "Polygon.h"

namespace mms {

class Mouse {

public:

    Mouse(const Maze* maze);

    // Get/set the crashed state of the mouse 
    bool didCrash() const;
    void setCrashed();

    // Resets the mouse to the beginning of the maze (taking into
    // account the desired starting direction, as set by the algorithm)
    void reset();

    // Sets the current translation and rotation of the mouse
    void teleport(const Coordinate& translation, const Angle& rotation);

    // Returns the direction of the mouse at the most recent reset
    Direction getStartedDirection() const;

    // Set the direction that the mouse should face whenever reset
    void setStartingDirection(Direction startingDirection);

    // Gets the initial translation of the mouse
    const Coordinate& getInitialTranslation() const;

    // Gets the current translation and rotation of the mouse
    const Coordinate& getCurrentTranslation() const;
    const Angle& getCurrentRotation() const;

    // Gets the current discretized translation and rotation of the mouse
    QPair<int, int> getCurrentDiscretizedTranslation() const;
    Direction getCurrentDiscretizedRotation() const;

    // Retrieves the polygon of just the body of the mouse
    Polygon getCurrentBodyPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;

    // Retrieves the polygons of the wheels of the robot
    Polygon getCurrentWheelPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;

    // Instruct the mouse to update its own position
    // based on how much simulation time has elapsed
    void update(const Duration& elapsed);

private:

    // Used for the sensor readings
    const Maze* m_maze;

    // Whether or not the mouse crashed
    bool m_crashed;

    // The direction that the mouse did and should face,
    // respectively, at the most recent and next reset
    Direction m_startedDirection;
    Direction m_startingDirection;

    // The translation and rotation of the mouse at the previous reload
    Coordinate m_initialTranslation;
    Angle m_initialRotation;

    // The parts of the mouse, as when positioned at m_initialTranslation and m_initialRotation
    Polygon m_initialBodyPolygon; // The polygon of strictly the body of the mouse
    Polygon m_initialWheelPolygon; // The wheels of the mouse

    // The gyro (rate of rotation), rotation, and translation
    // of the mouse, which change throughout execution
    // AngularVelocity m_currentGyro;
    Coordinate m_currentTranslation;
    Angle m_currentRotation;

    // Ensures that reads/updates happen atomically,
    // mutable so we can use it in const functions
    mutable QMutex m_mutex;

    // Helper function for polygon retrieval based on a given mouse translation and rotation
    Polygon getCurrentPolygon(
        const Polygon& initialPolygon,
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;
};

} 
