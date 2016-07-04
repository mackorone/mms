#pragma once

#include <QMap>
#include <QMapIterator>
#include <QSet>

#include "Assert.h"

namespace sim {

// TODO: MACK - use key() here
class ContainerUtilities {

public:

    // The ContainerUtilities class is not constructible
    ContainerUtilities() = delete;

    // Returns the inverse of a map, fails if there are dupliate values
    template<class K, class V>
    static QMap<V, K> inverse(const QMap<K, V>& map) {
        QSet<V> values;
        QMap<V, K> inverted;
        QMapIterator<K, V> iterator(map);
        while (iterator.hasNext()) {
            auto pair = iterator.next();
            SIM_ASSERT_FA(values.contains(pair.value()));
            inverted.insert(pair.value(), pair.key());
            values.insert(pair.value());
        }
        return inverted;
    }

};

} // namespace sim
