#pragma once

#include <map>
#include <string>

#include <pugixml.hpp>

namespace sim {

class ParamParser {

public:
    ParamParser(const std::string& filePath);

    // Check the existence and type of a value
    bool hasBoolValue(const std::string& tag);
    bool hasDoubleValue(const std::string& tag);
    bool hasIntValue(const std::string& tag);
    bool hasStringValue(const std::string& tag);

    // Retrieve the particular value
    bool getBoolValue(const std::string& tag);
    double getDoubleValue(const std::string& tag);
    int getIntValue(const std::string& tag);
    std::string getStringValue(const std::string& tag);

    // Get a value if we can, otherwise return a default
    bool getBoolIfHasBool(const std::string& tag, bool defaultValue);
    double getDoubleIfHasDouble(const std::string& tag, double defaultValue);
    int getIntIfHasInt(const std::string& tag, int defaultValue);
    std::string getStringIfHasString(const std::string& tag, const std::string& defaultValue);

    // If we can get a value and it's in the valid range then return it, else return default
    double getDoubleIfHasDoubleAndInRange(const std::string& tag, double defaultValue, double min, double max);
    int getIntIfHasIntAndInRange(const std::string& tag, int defaultValue, int min, int max);

    // If we can get a value and it's valid then return it, else return default
    std::string getStringIfHasStringAndIsColor(const std::string& tag, const std::string& defaultValue);
    std::string getStringIfHasStringAndIsDirection(const std::string& tag, const std::string& defaultValue);

private:
    pugi::xml_document m_doc;
    pugi::xml_parse_result m_result;

};

} // namespace sim
