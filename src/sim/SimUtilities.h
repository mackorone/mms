#pragma once

#include <algorithm>
#include <QPair>
#include <QString>
#include <QVector>

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

    // Returns a double in [0.0, 1.0]
    static double getRandom();

    // Sleeps the current thread for ms milliseconds
    static void sleep(const Duration& duration);

    // Like time() in <ctime> but higher resolution (returns seconds since epoch)
    static double getHighResTimestamp();

    // Converts a duration to a mm:ss.zzz string
    static QString formatDuration(const Duration& duration);

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

    // A simple pair-comparitor function
    template <class T>
    static bool lessThan(const QPair<T, T>& one, const QPair<T, T>& two) {
        return (one.first < two.first ? true : one.second < two.second);
    }

    // A simple pair-retrieval function
    template <class T>
    static QPair<T, T> min(const QVector<QPair<T, T>>& pairs) {
        return *std::min_element(pairs.begin(), pairs.end(), lessThan<T>);
    }

};

} // namespace mms
