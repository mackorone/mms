#pragma once

#include <QMap>
#include <QMapIterator>
#include <QPair>
#include <QVector>

#include "Assert.h"

namespace mms {

class ContainerUtilities {

public:

    // The ContainerUtilities class is not constructible
    ContainerUtilities() = delete;

    // Returns pairs of items in a map
    // TODO: MACK - GET RID OF THIS (OR DON"T USE IT IN THREADED CODE)
    template<class K, class V>
    static QVector<QPair<K, V>> items(QMap<K, V> map) {
        QVector<QPair<K, V>> items;
        QMapIterator<K, V> iterator(map);
        while (iterator.hasNext()) {
            auto pair = iterator.next();
            items.append({pair.key(), pair.value()});
        }
        return items;
    }

    // Returns the inverse of a map, fails if there are dupliate values
    // TODO: MACK - restrict when/where this can be used (don't make
    // unnecessary copies)
    template<class K, class V>
    static QMap<V, K> inverse(QMap<K, V> map) {
        QMap<V, K> inverted;
        QMapIterator<K, V> iterator(map);
        while (iterator.hasNext()) {
            auto pair = iterator.next();
            ASSERT_FA(inverted.contains(pair.value()));
            inverted.insert(pair.value(), pair.key());
        }
        return inverted;
    }

};

} // namespace mms
