#pragma once

#include <QMap>
#include <QMutex>
#include <QPair>
#include <QString>
#include <QVector>

#include "units/Cartesian.h"
#include "units/RadiansPerSecond.h"

#include "CurveTurnFactorCalculator.h"
#include "Direction.h"
#include "EncoderType.h"
#include "Maze.h"
#include "Polygon.h"
#include "Sensor.h"
#include "Wheel.h"
#include "WheelEffect.h"

namespace mms {

class Mouse {

public:
    Mouse(const Maze* maze);

    // Reloads the mouse (body, wheels, sensors, etc.) from the
    // given file; returns true if successful, false if not
    bool reload(const QString& mouseFile);

    // Returns the name of the current mouse file
    QString getMouseFile() const;

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
    Cartesian getInitialTranslation() const;

    // Gets the current translation and rotation of the mouse
    Cartesian getCurrentTranslation() const;
    Radians getCurrentRotation() const;

    // Gets the current discretized translation and rotation of the mouse
    QPair<int, int> getCurrentDiscretizedTranslation() const;
    Direction getCurrentDiscretizedRotation() const;

    // Retrieves the polygon of just the body of the mouse
    Polygon getCurrentBodyPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;

    // Retrieves the polygon comprised of all parts
    // of the mouse that could collide with walls
    Polygon getCurrentCollisionPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;

    // Retrieves the center of mass polygon of the mouse
    Polygon getCurrentCenterOfMassPolygon(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;

    // Retrieves the polygons of the wheels of the robot
    QVector<Polygon> getCurrentWheelPolygons(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;

    // Retrieves the speed indicator polygons of the wheels of the robot
    QVector<Polygon> getCurrentWheelSpeedIndicatorPolygons(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;

    // Retrieves the polygons of the sensors of the robot
    QVector<Polygon> getCurrentSensorPolygons(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;

    // Retrieve the polygons corresponding to the views of the sensors
    QVector<Polygon> getCurrentSensorViewPolygons(
        const Coordinate& currentTranslation,
        const Angle& currentRotation) const;

    // Instruct the mouse to update its own position
    // based on how much simulation time has elapsed
    void update(const Duration& elapsed);

    // Returns whether or not the mouse has a wheel by a particular name
    bool hasWheel(const QString& name) const;

    // Returns the magnitde of the max angular velocity of the wheel
    RadiansPerSecond getWheelMaxSpeed(const QString& name) const;

    // An atomic interface for setting the wheel speeds
    void setWheelSpeeds(const QMap<QString, RadiansPerSecond>& wheelSpeeds);

    // Helper methods for setting many wheel speeds at once, without having to
    // know the names of each of the wheels
    void setWheelSpeedsForMoveForward(double fractionOfMaxSpeed);
    void setWheelSpeedsForCurveLeft(double fractionOfMaxSpeed, const Meters& radius);
    void setWheelSpeedsForCurveRight(double fractionOfMaxSpeed, const Meters& radius);
    void stopAllWheels();

    // Returns the encoder type of the wheel given by name
    EncoderType getWheelEncoderType(const QString& name) const;

    // Returns the number of encoder ticks per revolution
    // for a particular wheel given by name
    double getWheelEncoderTicksPerRevolution(const QString& name) const;

    // Returns the reading of the encoder of the wheel given by name
    int readWheelAbsoluteEncoder(const QString& name) const;

    // Returns the reading of the encoder of the wheel given by name
    int readWheelRelativeEncoder(const QString& name) const;

    // Sets the value of the relative encoder to zero
    void resetWheelRelativeEncoder(const QString& name);

    // Returns whether or not the mouse has a sensor by a particular name
    bool hasSensor(const QString& name) const;

    // Read a sensor, and returns a value from 0.0
    // (completely free) to 1.0 (completely blocked)
    double readSensor(const QString& name) const;

    // Returns the value of the gyroscope
    RadiansPerSecond readGyro() const;

private:

    // Used for the sensor readings
    const Maze* m_maze;

    // The file that defines the current mouse geometry
    QString m_mouseFile;

    // The direction that the mouse did and should face,
    // respectively, at the most recent and next reset
    Direction m_startedDirection;
    Direction m_startingDirection;

    // The translation and rotation of the mouse at the previous reload
    Cartesian m_initialTranslation;
    Radians m_initialRotation;

    // The parts of the mouse, as when positioned at m_initialTranslation and m_initialRotation
    Polygon m_initialBodyPolygon; // The polygon of strictly the body of the mouse
    Polygon m_initialCollisionPolygon; // The polygon containing all collidable parts of the mouse
    Polygon m_initialCenterOfMassPolygon; // The polygon overlaying the center of mass of the mouse
    QMap<QString, Wheel> m_wheels; // The wheels of the mouse
    QMap<QString, Sensor> m_sensors; // The sensors on the mouse

    // The effect that each wheel has on mouse forward, sideways, and turn movements
    QMap<QString, WheelEffect> m_wheelEffects;
    QMap<QString, WheelEffect> getWheelEffects(
        const Cartesian& initialTranslation,
        const Radians& initialRotation,
        const QMap<QString, Wheel>& wheels) const;

    // The fractions of a each wheel's max speed that cause the mouse to
    // perform the move forward and turn movements, respectively, as optimally
    // as possible. Note that "as optimally as possible" is purposefully
    // ambiguous, because not all mice can move forward without turning or
    // moving sideways, and/or turn without moving forward or sideways.
    // Also note that the fractions are in [-1.0, 1.0], so that the max wheel
    // speed is never exceeded.
    QMap<QString, QPair<double, double>> m_wheelSpeedAdjustmentFactors;
    QMap<QString, QPair<double, double>> getWheelSpeedAdjustmentFactors(
        const QMap<QString, Wheel>& wheels,
        const QMap<QString, WheelEffect>& wheelEffects) const;

    // Used to calculate the linear combination of the forward component and turn
    // component, based on curve turn radius, that cause the mouse to perform a
    // curve turn as optimally as possible
    CurveTurnFactorCalculator m_curveTurnFactorCalculator;

    // The gyro (rate of rotation), rotation, and translation
    // of the mouse, which change throughout execution
    RadiansPerSecond m_currentGyro;
    Cartesian m_currentTranslation;
    Radians m_currentRotation;

    // Ensures that reads/updates happen atomically,
    // mutable so we can use it in const functions
    mutable QMutex m_mutex;

    // Helper function for polygon retrieval based on a given mouse translation and rotation
    Polygon getCurrentPolygon(
        const Polygon& initialPolygon,
        const Cartesian& currentTranslation,
        const Radians& currentRotation) const;

    // Retrieve the current position/rotation of sensor based on position/rotation of mouse
    QPair<Cartesian, Radians> getCurrentSensorPositionAndDirection(
        const Sensor& sensor,
        const Cartesian& currentTranslation,
        const Radians& currentRotation) const;

    // Sets the wheel speed for a particular movement, based on the linear combo of the two factors
    void setWheelSpeedsForMovement(double fractionOfMaxSpeed, double forwardFactor, double turnFactor);

};

} // namespace mms
