#pragma once

#include <QPair>
#include <QSet>

#include "units/Seconds.h"

namespace mms {

struct MouseStats {
    Seconds bestTimeToCenter = Seconds(-1);
    Seconds timeOfOriginDeparture = Seconds(-1);
    QSet<QPair<int, int>> traversedTileLocations;
    int closestDistanceToCenter = -1;
};

} // namespace mms
