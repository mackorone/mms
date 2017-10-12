#include "LayoutType.h"

#include "ContainerUtilities.h"

namespace mms {

const QMap<LayoutType, QString>& LAYOUT_TYPE_TO_STRING() {
    static const QMap<LayoutType, QString> map = {
        {LayoutType::FULL, "FULL"},
        {LayoutType::ZOOMED, "ZOOMED"},
    };
    return map;
}

const QMap<QString, LayoutType>& STRING_TO_LAYOUT_TYPE() {
    static const QMap<QString, LayoutType> map =
        ContainerUtilities::inverse(LAYOUT_TYPE_TO_STRING());
    return map;
}

} // namespace mms
