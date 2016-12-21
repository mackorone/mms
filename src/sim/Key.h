#pragma once

#include <QDebug>
#include <QMap>
#include <QString>
#include <QVector>

namespace mms {

enum class Key {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SPACE,
};

static const QVector<Key> ARROW_KEYS {
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

static const QMap<Key, QString> KEY_TO_STRING {
    {Key::LEFT, "LEFT"},
    {Key::RIGHT, "RIGHT"},
    {Key::UP, "UP"},
    {Key::DOWN, "DOWN"},
    {Key::SPACE, "SPACE"},
};

inline QDebug operator<<(QDebug stream, Key key) {
    stream.noquote() << KEY_TO_STRING.value(key);
    return stream;
}

} // namespace mms
