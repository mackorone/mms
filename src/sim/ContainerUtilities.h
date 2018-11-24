#pragma once

#include <QMap>
#include <type_traits>

#include "Assert.h"

namespace mms {

class ContainerUtilities {

public:

    ContainerUtilities() = delete;

    // Returns the inverse of a map, fails if there are dupliate values. Should
    // only be used as a convenience function for enum classes; if you need
    // this logic elsewhere, you should just use an iterator instead.
    template<typename K, typename V>
    static QMap<V, K> inverse(const QMap<K, V>& map) {
        ASSERT_TR(std::is_enum<K>::value);
        QMap<V, K> inverted;
        typename QMap<K, V>::const_iterator it;
        for (it = map.constBegin(); it != map.constEnd(); it += 1) {
            ASSERT_FA(inverted.contains(it.value()));
            inverted.insert(it.value(), it.key());
        }
        return inverted;
    }

};

} 
