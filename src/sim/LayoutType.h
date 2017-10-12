#pragma once

#include <QDebug>
#include <QMap>
#include <QString>

#include "ContainerUtilities.h"

namespace mms {

enum class LayoutType {
    FULL,
    ZOOMED,
};

const QMap<LayoutType, QString>& LAYOUT_TYPE_TO_STRING();
const QMap<QString, LayoutType>& STRING_TO_LAYOUT_TYPE();

inline QDebug operator<<(QDebug stream, LayoutType layoutType) {
    stream.noquote() << LAYOUT_TYPE_TO_STRING().value(layoutType);
    return stream;
}

} // namespace mms
