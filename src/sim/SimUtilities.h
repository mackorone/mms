#pragma once

#include <algorithm>
#include <QPair>
#include <QString>
#include <QVector>

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

    // Returns the UTC datetime formatted as YY-MM-DD_HH:MM:SS
    static QString timestampToDatetimeString(const Duration& timestamp);

    // Converts a duration to a mm:ss.zzz string
    static QString formatDuration(const Duration& duration);

    // Convert between types
    static bool isBool(const QString& str);
    static bool isInt(const QString& str);
    static bool isDouble(const QString& str);
    static bool strToBool(const QString& str);
    static int strToInt(const QString& str);
    static double strToDouble(const QString& str);

    // Returns all files contained anywhere within a directory
    static QPair<QStringList, QStringList> getFiles(const QString& dirPath);

    // Returns all dirs contained directly within a directory
    static QStringList getTopLevelDirs(const QString& dirPath);

    // Remove oldest runs from the run/ directory if necessary
    static void removeExcessArchivedRuns();

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
