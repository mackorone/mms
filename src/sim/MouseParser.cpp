#include "MouseParser.h"

#include <QDebug>
#include <QFile>
#include <QVector>

#include "Assert.h"
#include "EncoderType.h"
#include "GeometryUtilities.h"
#include "SimUtilities.h"
#include "units/AngularVelocity.h"

namespace mms {

const Polygon MouseParser::NULL_POLYGON = Polygon({
    Cartesian(Meters(0), Meters(0)),
    Cartesian(Meters(0), Meters(0)),
    Cartesian(Meters(0), Meters(0)),
});
const QString MouseParser::MOUSE_TAG = "Mouse";
const QString MouseParser::FORWARD_DIRECTION_TAG = "Forward-Direction";
const QString MouseParser::CENTER_OF_MASS_TAG = "Center-Of-Mass";
const QString MouseParser::BODY_TAG = "Body";
const QString MouseParser::VERTEX_TAG = "Vertex";
const QString MouseParser::X_TAG = "X";
const QString MouseParser::Y_TAG = "Y";
const QString MouseParser::WHEEL_TAG = "Wheel";
const QString MouseParser::NAME_TAG = "Name";
const QString MouseParser::DIAMETER_TAG = "Diameter";
const QString MouseParser::WIDTH_TAG = "Width";
const QString MouseParser::POSITION_TAG = "Position";
const QString MouseParser::DIRECTION_TAG = "Direction";
const QString MouseParser::MAX_SPEED_TAG = "Max-Speed";
const QString MouseParser::ENCODER_TYPE_TAG = "Encoder-Type";
const QString MouseParser::ENCODER_TICKS_PER_REVOLUTION_TAG = "Encoder-Ticks-Per-Revolution";
const QString MouseParser::SENSOR_TAG = "Sensor";
const QString MouseParser::RADIUS_TAG = "Radius";
const QString MouseParser::RANGE_TAG = "Range";
const QString MouseParser::HALF_WIDTH_TAG = "Half-Width";

MouseParser::MouseParser(const QString& filePath, bool* success) :
        m_forwardDirection(Angle::Radians(0)),
        m_centerOfMass(Cartesian(Meters(0), Meters(0))) {

    // Try to open the file
    QFile file(filePath);
    *success = file.open(QIODevice::ReadOnly);
    if (!*success) {
        qWarning() << "Unable to open file" << filePath;
        return;
    }

    // Try to read the file
    QString errorMessage;
    *success = m_doc.setContent(&file, false, &errorMessage);
    if (!*success) {
        qWarning() << "Unable to parse file" << filePath << ":" << errorMessage;
    }
    file.close();
    if (!*success) {
        return;
    }

    // Initialize some members of the class
    m_root = m_doc.firstChildElement(MOUSE_TAG);
    m_forwardDirection = Angle::Degrees(
        getDoubleIfHasDouble(m_root, FORWARD_DIRECTION_TAG, success));
    QDomElement centerOfMassElement = getContainerElement(m_root, CENTER_OF_MASS_TAG, success);
    double x = getDoubleIfHasDouble(centerOfMassElement, X_TAG, success);
    double y = getDoubleIfHasDouble(centerOfMassElement, Y_TAG, success);
    m_centerOfMass = Cartesian(Meters(x), Meters(y));
}

Polygon MouseParser::getBody(
        const Cartesian& initialTranslation,
        const Angle& initialRotation,
        bool* success) {

    Cartesian alignmentTranslation = initialTranslation - m_centerOfMass;
    Angle alignmentRotation = initialRotation - m_forwardDirection;

    QDomElement body = m_root.firstChildElement(BODY_TAG);
    if (body.isNull()) {
        qWarning() << "No" << BODY_TAG << "tag found.";
        *success = false;
    }

    QVector<Cartesian> vertices;
    QDomNodeList elementList = body.elementsByTagName(VERTEX_TAG);
    for (int i = 0; i < elementList.size(); i += 1) {
        QDomElement vertex = elementList.at(i).toElement();
        double x = getDoubleIfHasDouble(vertex, X_TAG, success);
        double y = getDoubleIfHasDouble(vertex, Y_TAG, success);
        vertices.push_back(
            alignVertex(
                Cartesian(Meters(x), Meters(y)),
                alignmentTranslation,
                alignmentRotation,
                initialTranslation));
    }

    if (vertices.size() < 3) {
        qWarning()
            << "Invalid mouse" << BODY_TAG
            << "- less than three valid vertices were specified.";
        *success = false;
    }

    Polygon bodyPolygon;
    if (success) {
        bodyPolygon = Polygon(vertices);
        if (bodyPolygon.getTriangles().size() == 0) {
            qWarning()
                << "Invalid mouse" << BODY_TAG
                << "- the vertices specified aren't a simple polygon.";
            *success = false;
        }
    }

    if (!*success) {
        return NULL_POLYGON;
    }
    return bodyPolygon;
}

QMap<QString, Wheel> MouseParser::getWheels(
        const Cartesian& initialTranslation,
        const Angle& initialRotation,
        bool* success) {

    Cartesian alignmentTranslation = initialTranslation - m_centerOfMass;
    Angle alignmentRotation = initialRotation - m_forwardDirection;

    QMap<QString, Wheel> wheels;
    QDomNodeList elementList = m_root.elementsByTagName(WHEEL_TAG);
    for (int i = 0; i < elementList.size(); i += 1) {
        QDomElement wheel = elementList.at(i).toElement();

        QString name = getNameIfNonemptyAndUnique("wheel", wheel, wheels, success);
        double diameter = getDoubleIfHasDoubleAndNonNegative(wheel, DIAMETER_TAG, success);
        double width = getDoubleIfHasDoubleAndNonNegative(wheel, WIDTH_TAG, success);
        QDomElement position = getContainerElement(wheel, POSITION_TAG, success);
        double x = getDoubleIfHasDouble(position, X_TAG, success);
        double y = getDoubleIfHasDouble(position, Y_TAG, success);
        double direction = getDoubleIfHasDouble(wheel, DIRECTION_TAG, success);
        double maxAngularVelocityMagnitude = getDoubleIfHasDoubleAndNonNegative(
            wheel, MAX_SPEED_TAG, success);
        EncoderType encoderType = getEncoderTypeIfValid(wheel, success);
        double encoderTicksPerRevolution = getDoubleIfHasDoubleAndNonNegative(
            wheel, ENCODER_TICKS_PER_REVOLUTION_TAG, success);

        if (success) {
            wheels.insert(
                name,
                Wheel(
                    Meters(diameter),
                    Meters(width),
                    alignVertex(
                        Cartesian(Meters(x), Meters(y)),
                        alignmentTranslation,
                        alignmentRotation,
                        initialTranslation),
                    Angle::Degrees(direction) + alignmentRotation,
                    AngularVelocity::RevolutionsPerMinute(maxAngularVelocityMagnitude),
                    encoderType, 
                    encoderTicksPerRevolution));
        }
    }

    return wheels;
}

QMap<QString, Sensor> MouseParser::getSensors(
        const Cartesian& initialTranslation,
        const Angle& initialRotation,
        const Maze& maze,
        bool* success) {

    Cartesian alignmentTranslation = initialTranslation - m_centerOfMass;
    Angle alignmentRotation = initialRotation - m_forwardDirection;

    QMap<QString, Sensor> sensors;
    QDomNodeList elementList = m_root.elementsByTagName(SENSOR_TAG);
    for (int i = 0; i < elementList.size(); i += 1) {
        QDomElement sensor = elementList.at(i).toElement();

        QString name = getNameIfNonemptyAndUnique("sensor", sensor, sensors, success);
        double radius = getDoubleIfHasDoubleAndNonNegative(sensor, RADIUS_TAG, success);
        double range = getDoubleIfHasDoubleAndNonNegative(sensor, RANGE_TAG, success);
        double halfWidth = getDoubleIfHasDoubleAndNonNegative(sensor, HALF_WIDTH_TAG, success);
        QDomElement position = getContainerElement(sensor, POSITION_TAG, success);
        double x = getDoubleIfHasDouble(position, X_TAG, success);
        double y = getDoubleIfHasDouble(position, Y_TAG, success);
        double direction = getDoubleIfHasDouble(sensor, DIRECTION_TAG, success);

        if (success) {
            sensors.insert(
                name,
                Sensor(
                    Meters(radius),
                    Meters(range), 
                    Angle::Degrees(halfWidth),
                    alignVertex(
                        Cartesian(Meters(x), Meters(y)),
                        alignmentTranslation,
                        alignmentRotation,
                        initialTranslation),
                    Angle::Degrees(direction) + alignmentRotation,
                    maze));
        }
    }

    return sensors;
}

double MouseParser::getDoubleIfHasDouble(const QDomElement& element, const QString& tag, bool* success) {
    QString valueString = element.firstChildElement(tag).text();
    if (!SimUtilities::isDouble(valueString)) {
        qWarning().noquote().nospace()
            << "Invalid value for tag \"" << tag << "\" - the tag is either"
            << " missing entirely, or its value isn't a valid floating point"
            << " number.";
        *success = false;
        return 0.0;
    }
    return SimUtilities::strToDouble(valueString);
}

double MouseParser::getDoubleIfHasDoubleAndNonNegative(
        const QDomElement& element, const QString& tag, bool* success) {
    double value = getDoubleIfHasDouble(element, tag, success);
    if (value < 0.0) {
        qWarning().noquote().nospace()
            << "The value for tag \"" << tag << "\" is " << value << ", which"
            << " is less than the minimum allowed value of " << 0.0 << ".";
        *success = false;
        return 0.0;
    }
    return value;
}

QDomElement MouseParser::getContainerElement(const QDomElement& element, const QString& tag, bool* success) {
    QDomElement containerElement = element.firstChildElement(tag);
    if (containerElement.isNull()) {
        qWarning()
            << "No wheel" << tag << "tag found. This means that the"
            << X_TAG << "and" << Y_TAG << "tags won't be found" << " either.";
        *success = false;
    }
    return containerElement;
}

EncoderType MouseParser::getEncoderTypeIfValid(const QDomElement& element, bool* success) {
    EncoderType encoderType;
    QString encoderTypeString = element.firstChildElement(ENCODER_TYPE_TAG).text();
    if (STRING_TO_ENCODER_TYPE().contains(encoderTypeString)) {
        encoderType = STRING_TO_ENCODER_TYPE().value(encoderTypeString);
    }
    else {
        qWarning().noquote().nospace()
            << "The encoder type \"" << encoderTypeString << "\" is not valid."
            << " The only valid encoder types are \""
            << ENCODER_TYPE_TO_STRING().value(EncoderType::ABSOLUTE)
            << "\" and \""
            << ENCODER_TYPE_TO_STRING().value(EncoderType::RELATIVE) << "\".";
        *success = false;
    }
    return encoderType;
}

Cartesian MouseParser::alignVertex(const Cartesian& vertex, const Cartesian& alignmentTranslation,
        const Angle& alignmentRotation, const Cartesian& rotationPoint) {
    Cartesian translated = GeometryUtilities::translateVertex(vertex, alignmentTranslation);
    return GeometryUtilities::rotateVertexAroundPoint(translated, alignmentRotation, rotationPoint);
}

} // namespace mms
