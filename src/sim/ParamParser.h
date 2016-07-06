#pragma once

#include <QMap>
#include <QString>

#include <pugixml/pugixml.hpp>

#include "Assert.h"

namespace sim {

class ParamParser {

public:
    ParamParser(const QString& filePath);

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
    // We have to keep m_doc around so valgrind doesn't complain
    pugi::xml_document m_doc;
    pugi::xml_node m_root;
    pugi::xml_parse_result m_fileIsReadable;

    static const QString PARAMETERS_TAG;

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
        SIM_ASSERT_TR(type == "int" || type == "double");
        SIM_ASSERT_LE(min, defaultValue);
        SIM_ASSERT_LE(defaultValue, max);
        if (!((type == "int" && hasIntValue(tag)) || (type == "double" && hasDoubleValue(tag)))) {
            printTagNotFound(type, tag, QString::number(defaultValue));
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

} // namespace sim
