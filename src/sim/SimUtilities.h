#pragma once

#include <QPair>
#include <QString>
#include <QStringList>
#include <QVector>

#include <algorithm>

#include "Color.h"
#include "Polygon.h"
#include "TriangleGraphic.h"
#include "units/Duration.h"

namespace mms {

class SimUtilities {

public:

    // The SimUtilities class is not constructible
    SimUtilities() = delete;

    // Quits the simulation
    static void quit();

    // Returns a random integer
    static int randomInt();

    // Returns a random integer
    static int randomNonNegativeInt(int max = 0);

    // Returns a double in [0.0, 1.0]
    static double getRandom();

    // Sleeps the current thread for ms milliseconds
    static void sleep(const Duration& duration);

    // Like time() in <ctime> but higher resolution (returns seconds since epoch)
    static double getHighResTimestamp();

    // Converts a duration to a mm:ss.zzz string
    static QString formatDuration(const Duration& duration);

    // Splits into lines in a cross-platform way
    static QStringList splitLines(const QString& string);

    // Convert between types
    static bool isBool(const QString& str);
    static bool isInt(const QString& str);
    static bool isDouble(const QString& str);
    static bool strToBool(const QString& str);
    static char strToChar(const QString& str);
    static int strToInt(const QString& str);
    static double strToDouble(const QString& str);
    static QString boolToStr(bool value);

    // Converts a polygon to a vector of triangle graphics
    static QVector<TriangleGraphic> polygonToTriangleGraphics(
        const Polygon& polygon,
        Color color,
        double alpha);

};

} 
