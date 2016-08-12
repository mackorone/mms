#pragma once

#include <QMap>
#include <QString>

#include "ContainerUtilities.h"

namespace mms {

enum class InterfaceType {
    DISCRETE,
    CONTINUOUS,
};

static const QMap<InterfaceType, QString> INTERFACE_TYPE_TO_STRING {
    {InterfaceType::DISCRETE, "DISCRETE"},
    {InterfaceType::CONTINUOUS, "CONTINUOUS"},
};

static const QMap<QString, InterfaceType> STRING_TO_INTERFACE_TYPE =
    ContainerUtilities::inverse(INTERFACE_TYPE_TO_STRING);

} // namespace mms
