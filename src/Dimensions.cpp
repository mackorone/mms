#include "Dimensions.h"

namespace mms {

Distance Dimensions::tileLength() { return wallLength() + wallWidth(); }

Distance Dimensions::halfTileLength() { return tileLength() / 2.0; }

Distance Dimensions::quarterTileLength() { return tileLength() / 4.0; }

Distance Dimensions::wallLength() { return Distance::Meters(0.168); }

Distance Dimensions::wallWidth() { return Distance::Meters(0.012); }

Distance Dimensions::halfWallWidth() { return wallWidth() / 2.0; }

}  // namespace mms
