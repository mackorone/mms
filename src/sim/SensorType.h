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

const QMap<SensorType, QString>& SENSOR_TYPE_TO_STRING();
const QMap<QString, SensorType>& STRING_TO_SENSOR_TYPE();

inline QDebug operator<<(QDebug stream, SensorType sensorType) {
    stream.noquote() << SENSOR_TYPE_TO_STRING().value(sensorType);
    return stream;
}

} // namespace mms
