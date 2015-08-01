#pragma once

#include <map>
#include <string>

#include <pugixml.hpp>

namespace sim {

class ParamParser {

public:
    ParamParser(std::string filePath);

    // Check the existence and type of a value
    bool hasBoolValue(std::string tag);
    bool hasDoubleValue(std::string tag);
    bool hasIntValue(std::string tag);
    bool hasStringValue(std::string tag);

    // Retrieve the particular value
    bool getBoolValue(std::string tag);
    double getDoubleValue(std::string tag);
    int getIntValue(std::string tag);
    std::string getStringValue(std::string tag);

    // Get a value if we can, otherwise return a default
    bool getBoolIfHasBool(std::string tag, bool defaultValue);
    double getDoubleIfHasDouble(std::string tag, double defaultValue);
    int getIntIfHasInt(std::string tag, int defaultValue);
    std::string getStringIfHasString(std::string tag, std::string defaultValue);

    // If we can get a value and it's in the valid range then return it, else return default
    double getDoubleIfHasDoubleAndInRange(std::string tag, double defaultValue, double min, double max);
    int getIntIfHasIntAndInRange(std::string tag, int defaultValue, int min, int max);

    // If we can get a value and it's in a key in the map then return it, else return default
    template<class T>
    std::string getStringIfHasStringAndIsKey(std::string tag, std::string defaultValue, std::map<std::string, T> map);

private:
    pugi::xml_document m_doc;

};

} // namespace sim
