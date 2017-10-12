#include "EncoderType.h"

#include "ContainerUtilities.h"

namespace mms {

const QMap<EncoderType, QString>& ENCODER_TYPE_TO_STRING() {
    static const QMap<EncoderType, QString> map = {
        {EncoderType::ABSOLUTE, "ABSOLUTE"},
        {EncoderType::RELATIVE, "RELATIVE"},
    };
    return map;
}

const QMap<QString, EncoderType>& STRING_TO_ENCODER_TYPE() {
    static const QMap<QString, EncoderType> map = 
        ContainerUtilities::inverse(ENCODER_TYPE_TO_STRING());
    return map;
}

} // namespace mms
