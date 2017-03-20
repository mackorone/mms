#pragma once

#include <QMap>
#include <QString>

#include <pugixml/pugixml.hpp>

#include "Assert.h"

namespace mms {

class ParamParser {

public:

    // TODO: MACK
    //ParamParser() = delete;
    //ParamParser();
    static void execEditDialog(); // TODO: MACK - shouldn't include window width

    // Check the existence and type of a value
    bool hasBoolValue(const QString& tag);
    bool hasDoubleValue(const QString& tag);
    bool hasIntValue(const QString& tag);
    bool hasCharValue(const QString& tag);
    bool hasStringValue(const QString& tag);

    // Retrieve the particular value
    bool getBoolValue(const QString& tag);
    double getDoubleValue(const QString& tag);
    int getIntValue(const QString& tag);
    char getCharValue(const QString& tag);
    QString getStringValue(const QString& tag);

    // Get a value if we can, otherwise return a default
    bool getBoolIfHasBool(const QString& tag, bool defaultValue);
    double getDoubleIfHasDouble(const QString& tag, double defaultValue);
    int getIntIfHasInt(const QString& tag, int defaultValue);
    char getCharIfHasChar(const QString& tag, char defaultValue);
    QString getStringIfHasString(const QString& tag, const QString& defaultValue);

    // If we can get a numeric value and it's valid then return it, else return default
    double getDoubleIfHasDoubleAndNotLessThan(const QString& tag, double defaultValue, double min);
    double getDoubleIfHasDoubleAndNotGreaterThan(const QString& tag, double defaultValue, double max);
    double getDoubleIfHasDoubleAndInRange(const QString& tag, double defaultValue, double min, double max);
    int getIntIfHasIntAndNotLessThan(const QString& tag, int defaultValue, int min);
    int getIntIfHasIntAndNotGreaterThan(const QString& tag, int defaultValue, int max);
    int getIntIfHasIntAndInRange(const QString& tag, int defaultValue, int min, int max);

    // If we can get a value and it's valid/special then return it, else return default
    QString getStringIfHasStringAndIsColor(const QString& tag, const QString& defaultValue);
    QString getStringIfHasStringAndIsDirection(const QString& tag, const QString& defaultValue);
    QString getStringIfHasStringAndIsLayoutType(const QString& tag, const QString& defaultValue);
    QString getStringIfHasStringAndIsMazeFileType(const QString& tag, const QString& defaultValue);
    QString getStringIfHasStringAndIsTileTextAlignment(const QString& tag, const QString& defaultValue);

private:

    // Settings group and helper functions
    static const QString GROUP_PREFIX;
    static QString getValue(const QString& key);
    static void setValue(const QString& key, const QString& value);


    void printTagNotFound(const QString& type, const QString& tag, const QString& defaultValue);
    void printLessThan(const QString& type, const QString& tag, const QString& value,
        const QString& defaultValue, const QString& min);
    void printGreaterThan(const QString& type, const QString& tag, const QString& value,
        const QString& defaultValue, const QString& max);
    void printNotSpecialString(const QString& type, const QString& tag,
        const QString& value, const QString& defaultValue);

    // Generic helper method for getting numeric values within a specific range
    template<class T>
    T getNumIfHasNumAndInRange(const QString& type, const QString& tag, T defaultValue, T min, T max) {
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
    QString getStringIfHasStringAndIsSpecial(
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
