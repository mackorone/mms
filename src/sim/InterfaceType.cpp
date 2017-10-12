#include "InterfaceType.h"

#include "ContainerUtilities.h"

namespace mms {

const QMap<InterfaceType, QString>& INTERFACE_TYPE_TO_STRING() {
    static const QMap<InterfaceType, QString> map = {
        {InterfaceType::DISCRETE, "DISCRETE"},
        {InterfaceType::CONTINUOUS, "CONTINUOUS"},
    };
    return map;
}

const QMap<QString, InterfaceType>& STRING_TO_INTERFACE_TYPE() {
    static const QMap<QString, InterfaceType> map =
        ContainerUtilities::inverse(INTERFACE_TYPE_TO_STRING());
    return map;
}

} // namespace mms
