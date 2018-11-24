#pragma once

#include <QPair>
#include <QSet>

#include "units/Duration.h"

namespace mms {

struct MouseStats {
    Duration bestTimeToCenter = Duration::Seconds(-1);
    Duration timeOfOriginDeparture = Duration::Seconds(-1);
    QSet<QPair<int, int>> traversedTileLocations;
    int closestDistanceToCenter = -1;
};

} 
