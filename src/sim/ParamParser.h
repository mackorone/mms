#pragma once

#include <map>
#include <string>

#include <pugixml/pugixml.hpp>

#include "Assert.h"
#include "ContainerUtilities.h"

namespace sim {

class ParamParser {

public:
    ParamParser(const std::string& filePath);

    // Check the existence and type of a value
    bool hasBoolValue(const std::string& tag);
    bool hasDoubleValue(const std::string& tag);
    bool hasIntValue(const std::string& tag);
    bool hasCharValue(const std::string& tag);
    bool hasStringValue(const std::string& tag);

    // Retrieve the particular value
    bool getBoolValue(const std::string& tag);
    double getDoubleValue(const std::string& tag);
    int getIntValue(const std::string& tag);
    char getCharValue(const std::string& tag);
    std::string getStringValue(const std::string& tag);

    // Get a value if we can, otherwise return a default
    bool getBoolIfHasBool(const std::string& tag, bool defaultValue);
    double getDoubleIfHasDouble(const std::string& tag, double defaultValue);
    int getIntIfHasInt(const std::string& tag, int defaultValue);
    char getCharIfHasChar(const std::string& tag, char defaultValue);
    std::string getStringIfHasString(const std::string& tag, const std::string& defaultValue);

    // If we can get a numeric value and it's valid then return it, else return default
    double getDoubleIfHasDoubleAndNotLessThan(const std::string& tag, double defaultValue, double min);
    double getDoubleIfHasDoubleAndNotGreaterThan(const std::string& tag, double defaultValue, double max);
    double getDoubleIfHasDoubleAndInRange(const std::string& tag, double defaultValue, double min, double max);
    int getIntIfHasIntAndNotLessThan(const std::string& tag, int defaultValue, int min);
    int getIntIfHasIntAndNotGreaterThan(const std::string& tag, int defaultValue, int max);
    int getIntIfHasIntAndInRange(const std::string& tag, int defaultValue, int min, int max);

    // If we can get a value and it's valid/special then return it, else return default
    std::string getStringIfHasStringAndIsColor(const std::string& tag, const std::string& defaultValue);
    std::string getStringIfHasStringAndIsDirection(const std::string& tag, const std::string& defaultValue);
    std::string getStringIfHasStringAndIsLayoutType(const std::string& tag, const std::string& defaultValue);
    std::string getStringIfHasStringAndIsMazeFileType(const std::string& tag, const std::string& defaultValue);
    std::string getStringIfHasStringAndIsTileTextAlignment(const std::string& tag, const std::string& defaultValue);

private:
    // We have to keep m_doc around so valgrind doesn't complain
    pugi::xml_document m_doc;
    pugi::xml_node m_root;
    pugi::xml_parse_result m_fileIsReadable;

    static const std::string PARAMETERS_TAG;

    void printTagNotFound(const std::string& type, const std::string& tag, const std::string& defaultValue);
    void printLessThan(const std::string& type, const std::string& tag, const std::string& value,
        const std::string& defaultValue, const std::string& min);
    void printGreaterThan(const std::string& type, const std::string& tag, const std::string& value,
        const std::string& defaultValue, const std::string& max);
    void printNotSpecialString(const std::string& type, const std::string& tag,
        const std::string& value, const std::string& defaultValue);

    // Generic helper method for getting numeric values within a specific range
    template<class T>
    T getNumIfHasNumAndInRange(const std::string& type, const std::string& tag, T defaultValue, T min, T max) {
        SIM_ASSERT_TR(type == "int" || type == "double");
        SIM_ASSERT_LE(min, defaultValue);
        SIM_ASSERT_LE(defaultValue, max);
        if (!((type == "int" && hasIntValue(tag)) || (type == "double" && hasDoubleValue(tag)))) {
            printTagNotFound(type, tag, std::to_string(defaultValue));
            return defaultValue;
        }
        T value = static_cast<T>(getDoubleValue(tag));
        if (value < min) {
            printLessThan(type, tag, std::to_string(value), std::to_string(defaultValue), std::to_string(min));
            return defaultValue;
        }
        else if (max < value) {
            printGreaterThan(type, tag, std::to_string(value), std::to_string(defaultValue), std::to_string(max));
            return defaultValue;
        }
        return value;
    }

    // Generic helper method for getting string values of a special type
    template<class T>
    std::string getStringIfHasStringAndIsSpecial(const std::string& type, const std::string& tag,
        const std::string& defaultValue, const std::map<std::string, T>& map) {
        if (!hasStringValue(tag)) {
            printTagNotFound("string", tag, defaultValue);
            return defaultValue;
        }
        std::string value = getStringValue(tag);
        if (!ContainerUtilities::mapContains(map, value)) {
            printNotSpecialString(type, tag, value, defaultValue);
            return defaultValue;
        }   
        return value;
    }

};

} // namespace sim
