#pragma once

#include <QMap>
#include <QString>

#include "ContainerUtilities.h"

namespace mms {

enum class LayoutType {
    FULL,
    ZOOMED,
};

static const QMap<LayoutType, LayoutType> LAYOUT_TYPE_CYCLE {
    {LayoutType::FULL, LayoutType::ZOOMED},
    {LayoutType::ZOOMED, LayoutType::FULL},
};

static const QMap<LayoutType, QString> LAYOUT_TYPE_TO_STRING {
    {LayoutType::FULL, "FULL"},
    {LayoutType::ZOOMED, "ZOOMED"},
};

static const QMap<QString, LayoutType> STRING_TO_LAYOUT_TYPE =
    ContainerUtilities::inverse(LAYOUT_TYPE_TO_STRING);

} // namespace mms
