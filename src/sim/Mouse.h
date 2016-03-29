#pragma once

#include <mutex>
#include <vector>

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

namespace sim {

class Mouse {

public:
    Mouse(const Maze* maze);

    // Initializes the mouse (body, wheels, sensors, etc.); returns true if successful, false if not
    bool initialize(
        const std::string& mouseFile,
        Direction initialDirection);

    // Gets the initial translation and rotation of the mouse
    Cartesian getInitialTranslation() const;
    Radians getInitialRotation() const;

    // Gets the current translation and rotation of the mouse
    Cartesian getCurrentTranslation() const;
    Radians getCurrentRotation() const;

    // Gets the current discretized translation and rotation of the mouse
    std::pair<int, int> getCurrentDiscretizedTranslation() const;
    Direction getCurrentDiscretizedRotation() const;

    // Sets the current translation and rotation of the mouse
    void teleport(const Coordinate& translation, const Angle& rotation);

    // Retrieves the polygon of just the body of the mouse
    Polygon getCurrentBodyPolygon(
        const Coordinate& currentTranslation, const Angle& currentRotation) const;

    // Retrieves the polygon comprised of all parts of the mouse that could collide with walls
    Polygon getCurrentCollisionPolygon(
        const Coordinate& currentTranslation, const Angle& currentRotation) const;

    // Retrieves the center of mass polygon of the mouse
    Polygon getCurrentCenterOfMassPolygon(
        const Coordinate& currentTranslation, const Angle& currentRotation) const;

    // Retrieves the polygons of the wheels of the robot
    std::vector<Polygon> getCurrentWheelPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const;

    // Retrieves the speed indicator polygons of the wheels of the robot
    std::vector<Polygon> getCurrentWheelSpeedIndicatorPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const;

    // Retrieves the polygons of the sensors of the robot
    std::vector<Polygon> getCurrentSensorPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const;

    // Retrieve the polygons corresponding to the views of the sensors
    std::vector<Polygon> getCurrentSensorViewPolygons(
        const Coordinate& currentTranslation, const Angle& currentRotation) const;

    // Instruct the mouse to update its own position based on how much simulation time has elapsed
    void update(const Duration& elapsed);

    // Returns whether or not the mouse has a wheel by a particular name
    bool hasWheel(const std::string& name) const;

    // Returns the magnitde of the max angular velocity of the wheel
    RadiansPerSecond getWheelMaxSpeed(const std::string& name) const;

    // An atomic interface for setting the wheel speeds
    void setWheelSpeeds(const std::map<std::string, RadiansPerSecond>& wheelSpeeds);

    // Helper methods for setting many wheel speeds at once, without having to
    // know the names of each of the wheels
    void setWheelSpeedsForMoveForward(double fractionOfMaxSpeed);
    void setWheelSpeedsForCurveLeft(double fractionOfMaxSpeed, const Meters& radius);
    void setWheelSpeedsForCurveRight(double fractionOfMaxSpeed, const Meters& radius);
    void stopAllWheels();

    // Returns the encoder type of the wheel given by name
    EncoderType getWheelEncoderType(const std::string& name) const;

    // Returns the number of encoder ticks per revolution for a particular wheel given by name
    double getWheelEncoderTicksPerRevolution(const std::string& name) const;

    // Returns the reading of the encoder of the wheel given by name
    int readWheelAbsoluteEncoder(const std::string& name) const;

    // Returns the reading of the encoder of the wheel given by name
    int readWheelRelativeEncoder(const std::string& name) const;

    // Sets the value of the relative encoder to zero
    void resetWheelRelativeEncoder(const std::string& name);

    // Returns whether or not the mouse has a sensor by a particular name
    bool hasSensor(const std::string& name) const;

    // Read a sensor, and returns a value from 0.0 (completely free) to 1.0 (completely blocked)
    double readSensor(const std::string& name) const;

    // Returns the value of the gyroscope
    RadiansPerSecond readGyro() const;

private:
    // Used for the sensor readings
    const Maze* m_maze;

    // The initial translation and rotation of the mouse
    Cartesian m_initialTranslation;
    Radians m_initialRotation;

    // The parts of the mouse, as when positioned at m_initialTranslation and m_initialRotation
    Polygon m_initialBodyPolygon; // The polygon of strictly the body of the mouse
    Polygon m_initialCollisionPolygon; // The polygon containing all collidable parts of the mouse
    Polygon m_initialCenterOfMassPolygon; // The polygon overlaying the center of mass of the mouse
    std::map<std::string, Wheel> m_wheels; // The wheels of the mouse
    std::map<std::string, Sensor> m_sensors; // The sensors on the mouse

    // The effect that each wheel has on mouse forward, sideways, and turn movements
    std::map<std::string, WheelEffect> m_wheelEffects;

    // The fractions of a each wheel's max speed that cause the mouse to
    // perform the move forward and turn movements, respectively, as optimally
    // as possible. Note that "as optimally as possible" is purposefully
    // ambiguous, because not all mice can move forward without turning or
    // moving sideways, and/or turn without moving forward or sideways.
    // Also note that the fractions are in [-1.0, 1.0], so that the max wheel
    // speed is never exceeded.
    std::map<std::string, std::pair<double, double>> m_wheelSpeedAdjustmentFactors;

    // Used to calculate the linear combination of the forward component and turn
    // component, based on curve turn radius, that cause the mouse to perform a
    // curve turn as optimally as possible
    CurveTurnFactorCalculator m_curveTurnFactorCalculator;

    // The gyro (rate of rotation), rotation, and translation of the mouse,
    // which change throughout execution
    RadiansPerSecond m_currentGyro;
    Cartesian m_currentTranslation;
    Radians m_currentRotation;

    // Ensures that updates happen atomically, mutable so we can use it in const functions
    mutable std::mutex m_updateMutex; 

    // Helper function for polygon retrieval based on a given mouse translation and rotation
    Polygon getCurrentPolygon(const Polygon& initialPolygon,
        const Cartesian& currentTranslation, const Radians& currentRotation) const;

    // Retrieve the current position/rotation of sensor based on position/rotation of mouse
    std::pair<Cartesian, Radians> getCurrentSensorPositionAndDirection(
        const Sensor& sensor,
        const Cartesian& currentTranslation,
        const Radians& currentRotation) const;

    // Sets the wheel speed for a particular movement, based on the linear combo of the two factors
    void setWheelSpeedsForMovement(double fractionOfMaxSpeed, double forwardFactor, double turnFactor);

    // Helper method for getting forward and turn rates of change due to a single wheel
    std::map<std::string, WheelEffect> getWheelEffects(
        const Cartesian& initialTranslation,
        const Radians& initialRotation,
        const std::map<std::string, Wheel>& wheels) const;

    // Helper method for getting wheel speed adjustment factors based on wheel effects
    std::map<std::string, std::pair<double, double>> getWheelSpeedAdjustmentFactors(
        const std::map<std::string, Wheel>& wheels,
        const std::map<std::string, WheelEffect>& wheelEffects) const;

};

} // namespace sim
