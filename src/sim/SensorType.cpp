#include "SensorType.h"

#include "ContainerUtilities.h"

namespace mms {

const QMap<SensorType, QString>& SENSOR_TYPE_TO_STRING() {
    static const QMap<SensorType, QString> map = {
        {SensorType::ANALOG, "ANALOG"},
        {SensorType::DIGITAL, "DIGITAL"},
    };
    return map;
}

const QMap<QString, SensorType>& STRING_TO_SENSOR_TYPE() {
    static const QMap<QString, SensorType> map =
        ContainerUtilities::inverse(SENSOR_TYPE_TO_STRING());
    return map;
}

} // namespace mms
