#pragma once

#include <QChar>
#include <QString>
#include <QVector>

#include "Color.h"
#include "Polygon.h"
#include "TriangleGraphic.h"

namespace mms {

class SimUtilities {

public:

    // The SimUtilities class is not constructible
    SimUtilities() = delete;

    // Like time() in <ctime> but higher resolution (returns seconds since epoch)
    static double getHighResTimestamp();

    // Convert between types
    static bool isBool(const QString& str);
    static bool isInt(const QString& str);
    static bool strToBool(const QString& str);
    static QChar strToChar(const QString& str);
    static int strToInt(const QString& str);
    static QString boolToStr(bool value);

    // Converts a polygon to a vector of triangle graphics
    static QVector<TriangleGraphic> polygonToTriangleGraphics(
        const Polygon& polygon,
        Color color,
        unsigned char alpha);

};

} 
