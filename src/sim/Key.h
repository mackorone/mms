#pragma once

#include <QMap>
#include <QVector>

namespace mms {

enum class Key {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SPACE,
};

static const QVector<Key> ARROW_KEYS = {
    Key::LEFT,
    Key::RIGHT,
    Key::UP,
    Key::DOWN,
};

static const QMap<int, Key> INT_TO_KEY {
    { 32, Key::SPACE},
    {100, Key::LEFT},
    {102, Key::RIGHT},
    {101, Key::UP},
    {103, Key::DOWN},
};

} // namespace mms
