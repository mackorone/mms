#pragma once

#include <QDebug>
#include <QMap>
#include <QString>

#include "RGB.h"

namespace mms {

enum class Color {
    BLACK,
    BLUE,
    CYAN,
    GRAY,
    GREEN,
    ORANGE,
    RED,
    WHITE,
    YELLOW,
    DARK_BLUE,
    DARK_CYAN,
    DARK_GRAY,
    DARK_GREEN,
    DARK_ORANGE,
    DARK_RED,
    DARK_VIOLET,
    DARK_YELLOW,
};

const QMap<Color, RGB>& COLOR_TO_RGB();

const QMap<Color, QString>& COLOR_TO_STRING();
const QMap<QString, Color>& STRING_TO_COLOR();

const QMap<Color, char>& COLOR_TO_CHAR();
const QMap<char, Color>& CHAR_TO_COLOR();

inline QDebug operator<<(QDebug stream, Color color) {
    stream.noquote() << COLOR_TO_STRING().value(color);
    return stream;
}

} // namespace mms
