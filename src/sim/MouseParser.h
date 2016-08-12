#pragma once

#include <QDebug>
#include <QMap>
#include <QString>

#include <pugixml/pugixml.hpp>
#include <QString>
#include <QVector>

#include "Logging.h"
#include "Maze.h"
#include "Polygon.h"
#include "Sensor.h"
#include "units/Cartesian.h"
#include "units/Meters.h"
#include "Wheel.h"

namespace mms {

class MouseParser {

public:

    MouseParser(const QString& filePath, bool* success);

    Polygon getBody(
        const Cartesian& initialTranslation,
        const Radians& initialRotation,
        bool* success);

    QMap<QString, Wheel> getWheels(
        const Cartesian& initialTranslation,
        const Radians& initialRotation,
        bool* success);

    QMap<QString, Sensor> getSensors(
        const Cartesian& initialTranslation,
        const Radians& initialRotation,
        const Maze& maze,
        bool* success);

private:
    // We have to keep m_doc around so valgrind doesn't complain
    pugi::xml_document m_doc;
    pugi::xml_node m_root;
    Radians m_forwardDirection;
    Cartesian m_centerOfMass;

    double getDoubleIfHasDouble(const pugi::xml_node& node, const QString& tag, bool* success);
    double getDoubleIfHasDoubleAndNonNegative(
        const pugi::xml_node& node, const QString& tag, bool* success);
    pugi::xml_node getContainerNode(const pugi::xml_node& node, const QString& tag, bool* success);
    EncoderType getEncoderTypeIfValid(const pugi::xml_node& node, bool* success);

    Cartesian alignVertex(const Cartesian& vertex, const Cartesian& alignmentTranslation,
        const Radians& alignmentRotation, const Cartesian& rotationPoint);

    static const Polygon NULL_POLYGON;
    static const QString MOUSE_TAG;
    static const QString FORWARD_DIRECTION_TAG;
    static const QString CENTER_OF_MASS_TAG;
    static const QString BODY_TAG;
    static const QString VERTEX_TAG;
    static const QString X_TAG;
    static const QString Y_TAG;
    static const QString WHEEL_TAG;
    static const QString NAME_TAG;
    static const QString DIAMETER_TAG;
    static const QString WIDTH_TAG;
    static const QString POSITION_TAG;
    static const QString DIRECTION_TAG;
    static const QString MAX_SPEED_TAG;
    static const QString ENCODER_TYPE_TAG;
    static const QString ENCODER_TICKS_PER_REVOLUTION_TAG;
    static const QString SENSOR_TAG;
    static const QString RADIUS_TAG;
    static const QString RANGE_TAG;
    static const QString HALF_WIDTH_TAG;

    template<class T>
    QString getNameIfNonemptyAndUnique(const QString& type,
            const pugi::xml_node& node, const QMap<QString, T>& map, bool* success) {
        QString name = node.child(NAME_TAG.toStdString().c_str()).child_value();
        if (name.isEmpty()) {
            qWarning().noquote().nospace()
                << "No " << type << " name specified.";
            *success = false;
        }
        if (map.contains(name)) {
            qWarning().noquote().nospace()
                << "Two " << type << "s both have the name \"" << name << "\".";
            *success = false;
        }
        return name;
    }
};

} // namespace mms
