#pragma once

#include <map>

#include "ContainerUtilities.h"

// Windows already has these defined
#undef ABSOLUTE
#undef RELATIVE

namespace sim {

enum class EncoderType { ABSOLUTE, RELATIVE };

static const std::map<EncoderType, std::string> ENCODER_TYPE_TO_STRING {
    {EncoderType::ABSOLUTE, "ABSOLUTE"},
    {EncoderType::RELATIVE, "RELATIVE"},
};

static const std::map<std::string, EncoderType> STRING_TO_ENCODER_TYPE  = 
    ContainerUtilities::inverse(ENCODER_TYPE_TO_STRING);

} // namespace sim
