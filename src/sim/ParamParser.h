#pragma once

#include <QMap>
#include <QString>

#include "Assert.h"

namespace mms {

class ParamParser {

public:

    ParamParser() = delete;
    static void execEditDialog(); // TODO: MACK - shouldn't include window width

    // Check the existence and type of a value
    static bool hasBoolValue(const QString& tag);
    static bool hasDoubleValue(const QString& tag);
    static bool hasIntValue(const QString& tag);
    static bool hasCharValue(const QString& tag);
    static bool hasStringValue(const QString& tag);

    // Retrieve the particular value
    static bool getBoolValue(const QString& tag);
    static double getDoubleValue(const QString& tag);
    static int getIntValue(const QString& tag);
    static char getCharValue(const QString& tag);
    static QString getStringValue(const QString& tag);

    // Get a value if we can, otherwise return a default
    static bool getBoolIfHasBool(const QString& tag, bool defaultValue);
    static double getDoubleIfHasDouble(const QString& tag, double defaultValue);
    static int getIntIfHasInt(const QString& tag, int defaultValue);
    static char getCharIfHasChar(const QString& tag, char defaultValue);
    static QString getStringIfHasString(const QString& tag, const QString& defaultValue);

    // If we can get a numeric value and it's valid then return it, else return default
    static double getDoubleIfHasDoubleAndNotLessThan(const QString& tag, double defaultValue, double min);
    static double getDoubleIfHasDoubleAndNotGreaterThan(const QString& tag, double defaultValue, double max);
    static double getDoubleIfHasDoubleAndInRange(const QString& tag, double defaultValue, double min, double max);
    static int getIntIfHasIntAndNotLessThan(const QString& tag, int defaultValue, int min);
    static int getIntIfHasIntAndNotGreaterThan(const QString& tag, int defaultValue, int max);
    static int getIntIfHasIntAndInRange(const QString& tag, int defaultValue, int min, int max);

    // If we can get a value and it's valid/special then return it, else return default
    static QString getStringIfHasStringAndIsColor(const QString& tag, const QString& defaultValue);
    static QString getStringIfHasStringAndIsDirection(const QString& tag, const QString& defaultValue);
    static QString getStringIfHasStringAndIsLayoutType(const QString& tag, const QString& defaultValue);
    static QString getStringIfHasStringAndIsMazeFileType(const QString& tag, const QString& defaultValue);
    static QString getStringIfHasStringAndIsTileTextAlignment(const QString& tag, const QString& defaultValue);

private:

    // Settings group and helper functions
    static const QString GROUP_PREFIX;
    static QString getValue(const QString& key);
    static void setValue(const QString& key, const QString& value);


    static void printTagNotFound(const QString& type, const QString& tag, const QString& defaultValue);
    static void printLessThan(const QString& type, const QString& tag, const QString& value,
        const QString& defaultValue, const QString& min);
    static void printGreaterThan(const QString& type, const QString& tag, const QString& value,
        const QString& defaultValue, const QString& max);
    static void printNotSpecialString(const QString& type, const QString& tag,
        const QString& value, const QString& defaultValue);

    // Generic helper method for getting numeric values within a specific range
    template<class T>
    static T getNumIfHasNumAndInRange(const QString& type, const QString& tag, T defaultValue, T min, T max) {
        ASSERT_TR(type == "int" || type == "double");
        ASSERT_LE(min, defaultValue);
        ASSERT_LE(defaultValue, max);
        if (!((type == "int" && hasIntValue(tag)) || (type == "double" && hasDoubleValue(tag)))) {
            printTagNotFound(type, tag, QString::number(defaultValue));
            setValue(tag, QString::number(defaultValue));
            return defaultValue;
        }
        T value = static_cast<T>(getDoubleValue(tag));
        if (value < min) {
            printLessThan(type, tag, QString::number(value), QString::number(defaultValue), QString::number(min));
            return defaultValue;
        }
        else if (max < value) {
            printGreaterThan(type, tag, QString::number(value), QString::number(defaultValue), QString::number(max));
            return defaultValue;
        }
        return value;
    }

    // Generic helper method for getting string values of a special type
    template<class T>
    static QString getStringIfHasStringAndIsSpecial(
            const QString& type,
            const QString& tag,
            const QString& defaultValue,
            const QMap<QString, T>& map) {

        if (!hasStringValue(tag)) {
            printTagNotFound("string", tag, defaultValue);
            setValue(tag, defaultValue);
            return defaultValue;
        }
        QString value = getStringValue(tag);
        if (!map.contains(value)) {
            printNotSpecialString(type, tag, value, defaultValue);
            return defaultValue;
        }   
        return value;
    }

};

} // namespace mms
