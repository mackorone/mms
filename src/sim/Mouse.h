#pragma once

#include <mutex>
#include <vector>

#include "units/Cartesian.h"
#include "units/RadiansPerSecond.h"

#include "Maze.h"
#include "Polygon.h"
#include "Sensor.h"
#include "Wheel.h"

namespace sim {

class Mouse {

public:
    Mouse(const Maze* maze);

    // Initializes the mouse (body, wheels, sensors, etc.); returns true if successful, false if not
    bool initialize(const std::string& mouseFile);

    // Retrieves the polygon of just the body of the mouse
    Polygon getBodyPolygon() const;

    // Retrieves the polygon comprised of all parts of the mouse that could collide with walls
    Polygon getCollisionPolygon() const;

    // Retrieves the center of mass polygon of the mouse
    Polygon getCenterOfMassPolygon() const;

    // Retrieves the polygons of the wheels of the robot
    std::vector<Polygon> getWheelPolygons() const;

    // Retrieves the speed indicator polygons of the wheels of the robot
    std::vector<Polygon> getWheelSpeedIndicatorPolygons() const;

    // Retrieves the polygons of the sensors of the robot
    std::vector<Polygon> getSensorPolygons() const;

    // Retrieve the polygons corresponding to the views of the sensors
    std::vector<Polygon> getViewPolygons() const;

    // Instruct the mouse to update its own position based on how much simulation time has elapsed
    void update(const Duration& elapsed);

    // Returns whether or not the mouse has a wheel by a particular name
    bool hasWheel(const std::string& name) const;

    // An atomic interface for setting the wheel speeds
    void setWheelSpeeds(const std::map<std::string, RadiansPerSecond>& wheelSpeeds);

    // Returns whether or not the mouse has a sensor by a particular name
    bool hasSensor(const std::string& name) const;

    // Read a sensor, and returns a value from 0.0 (completely free) to 1.0 (completely blocked)
    double readSensor(const std::string& name) const;

    // Get the read time of a particular sensor
    Seconds getSensorReadDuration(const std::string& name) const;

    // Returns the value of the gyroscope
    RadiansPerSecond readGyro() const;

    // Discretized information, used by MouseInterface
    std::pair<int, int> getDiscretizedTranslation() const;
    Direction getDiscretizedRotation() const;

    // For use with the discrete interface *only*
    Cartesian getInitialTranslation() const;
    Cartesian getCurrentTranslation() const;
    Radians getCurrentRotation() const;
    void teleport(const Cartesian& translation, const Angle& rotation);

private:
    // Used for the sensor readings
    const Maze* m_maze;

    // The mouse, as it's positioned at the start execution
    Cartesian m_initialTranslation; // Always the center of the starting tile
    Polygon m_initialBodyPolygon; // The polygon of strictly the body of the mouse
    Polygon m_initialCollisionPolygon; // The polygon containing all collidable parts of the mouse
    Polygon m_initialCenterOfMassPolygon; // The polygon overlaying the center of mass of the mouse

    // The gyro, rotation, and translation of the mouse, which change throughout execution
    RadiansPerSecond m_gyro;
    Radians m_rotation;
    Cartesian m_translation;

    // The wheels of the mouse
    std::map<std::string, Wheel> m_wheels;

    // The sensors on the mouse
    std::map<std::string, Sensor> m_sensors;

    // Ensures the wheel speeds are accessed together atomically
    std::mutex m_wheelMutex; 

    // Helper function for getWheelPolygons and getSensorPolygons
    template<class T>
    std::vector<Polygon> getPolygonsFromMap(std::map<std::string, T> map) const {
        std::vector<Polygon> initialPolygons;
        for (std::pair<std::string, T> pair : map) {
            initialPolygons.push_back(pair.second.getInitialPolygon());
        }
        std::vector<Polygon> adjustedPolygons;
        for (Polygon polygon : initialPolygons) {
            adjustedPolygons.push_back(
                polygon.translate(m_translation - m_initialTranslation).rotateAroundPoint(m_rotation, m_translation));
        }
        return adjustedPolygons;
    }
};

} // namespace sim
