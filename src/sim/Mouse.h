#pragma once

#include <mutex>
#include <vector>

#include "units/Cartesian.h"
#include "units/RadiansPerSecond.h"

#include "Direction.h"
#include "EncoderType.h"
#include "InterfaceType.h"
#include "Maze.h"
#include "Polygon.h"
#include "Sensor.h"
#include "Wheel.h"

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
    void setWheelSpeedsForTurnLeft(double fractionOfMaxSpeed);
    void setWheelSpeedsForTurnRight(double fractionOfMaxSpeed);
    void stopAllWheels();

    // Returns the encoder type of the wheel given by name
    EncoderType getWheelEncoderType(const std::string& name) const;

    // Returns the number of encoder ticks per revolution for a particular wheel given by name
    double getWheelEncoderTicksPerRevolution(const std::string& name) const;

    // Returns the reading of the encoder of the wheel given by name
    int readWheelAbsoluteEncoder(const std::string& name);

    // Returns the reading of the encoder of the wheel given by name
    int readWheelRelativeEncoder(const std::string& name);

    // Sets the value of the relative encoder to zero
    void resetWheelRelativeEncoder(const std::string& name);

    // Returns whether or not the mouse has a sensor by a particular name
    bool hasSensor(const std::string& name) const;

    // Read a sensor, and returns a value from 0.0 (completely free) to 1.0 (completely blocked)
    double readSensor(const std::string& name) const;

    // Get the read time of a particular sensor
    Seconds getSensorReadDuration(const std::string& name) const;

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

    // The gyro (rate of rotation), rotation, and translation of the mouse,
    // which change throughout execution
    RadiansPerSecond m_currentGyro;
    Cartesian m_currentTranslation;
    Radians m_currentRotation;

    // Ensures that updates happen atomically
    std::mutex m_updateMutex; 

    // Helper function for polygon retrieval based on a given mouse translation and rotation
    Polygon getCurrentPolygon(const Polygon& initialPolygon,
        const Cartesian& currentTranslation, const Radians& currentRotation) const;

    // Helper function for retrieving the polygon corresponding to the view of a sensor
    Polygon getCurrentSensorViewPolygon(const Sensor& sensor,
        const Cartesian& currentTranslation, const Radians& currentRotation) const;

    // Get the forward and radial contribution factors for a wheel
    std::pair<double, double> getWheelContributionFactors(const std::string& name) const;
};

} // namespace sim
