#pragma once

#include <QDebug>
#include <QMap>
#include <QString>

#include "ContainerUtilities.h"

namespace mms {

enum class SensorType {
    ANALOG,
    DIGITAL,
};

static const QMap<SensorType, QString> SENSOR_TYPE_TO_STRING {
    {SensorType::ANALOG, "ANALOG"},
    {SensorType::DIGITAL, "DIGITAL"},
};

static const QMap<QString, SensorType> STRING_TO_SENSOR_TYPE =
    ContainerUtilities::inverse(SENSOR_TYPE_TO_STRING);

inline QDebug operator<<(QDebug stream, SensorType sensorType) {
    stream.noquote() << SENSOR_TYPE_TO_STRING.value(sensorType);
    return stream;
}

} // namespace mms
