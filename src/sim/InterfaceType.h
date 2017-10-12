#pragma once

#include <QDebug>
#include <QMap>
#include <QString>

#include "ContainerUtilities.h"

namespace mms {

enum class InterfaceType {
    DISCRETE,
    CONTINUOUS,
};

const QMap<InterfaceType, QString>& INTERFACE_TYPE_TO_STRING();
const QMap<QString, InterfaceType>& STRING_TO_INTERFACE_TYPE();

inline QDebug operator<<(QDebug stream, InterfaceType interfaceType) {
    stream.noquote() << INTERFACE_TYPE_TO_STRING().value(interfaceType);
    return stream;
}

} // namespace mms
