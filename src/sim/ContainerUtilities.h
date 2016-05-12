#pragma once

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "Assert.h"

namespace sim {

class ContainerUtilities {

public:

    // The ContainerUtilities class is not constructible
    ContainerUtilities() = delete;

    // Returns whether or not a vector contains a particular item
    template<class T>
    static bool vectorContains(const std::vector<T>& vector, T item) {
        return std::find(vector.begin(), vector.end(), item) != vector.end();
    }

    // Returns whether or not a set contains a particular key
    template<class T>
    static bool setContains(const std::set<T>& set, T item) {
        return set.find(item) != set.end();
    }

    // Returns whether or not a map contains a particular key
    template<class K, class V>
    static bool mapContains(const std::map<K, V>& map, K key) {
        return map.find(key) != map.end();
    }

    // Returns the keys of a map
    template<class K, class V>
    static std::set<K> keys(const std::map<K, V>& map) {
        return kvs(map).first;
    }

    // Returns the values of a map
    template<class K, class V>
    static std::set<V> values(const std::map<K, V>& map) {
        return kvs(map).second;
    }

    // Returns the inverse of a map, fails if there are dupliate values
    template<class K, class V>
    static std::map<V, K> inverse(const std::map<K, V>& map) {
        std::set<V> values;
        std::map<V, K> inverted;
        for (auto it = map.begin(); it != map.end(); it++) {
            SIM_ASSERT_FA(setContains(values, it->second));
            inverted.insert(std::make_pair(it->second, it->first));
            values.insert(it->second);
        }
        return inverted;
    }

private:

    // Returns the keys and values of a map
    template<class K, class V>
    static std::pair<std::set<K>, std::set<V>> kvs(const std::map<K, V>& map) {
        std::set<K> keys;
        std::set<V> values;
        for (auto it = map.begin(); it != map.end(); it++) {
            keys.insert(it->first);
            values.insert(it->second);
        }
        return std::make_pair(keys, values);
    }

};

} // namespace sim
