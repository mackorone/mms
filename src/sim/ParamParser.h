#pragma once

#include <string>

#include <pugixml.hpp>

namespace sim {

class ParamParser {

public:
    ParamParser(std::string filePath);

    // Check the existence and type of a value
    bool hasBoolValue(std::string tag);
    bool hasFloatValue(std::string tag);
    bool hasIntValue(std::string tag);
    bool hasStringValue(std::string tag);

    // Retrieve the particular value
    bool getBoolValue(std::string tag);
    float getFloatValue(std::string tag);
    int getIntValue(std::string tag);
    std::string getStringValue(std::string tag);

    // Get a value if we can, otherwise return a default
    bool getBoolIfHasBool(std::string tag, bool defaultValue);
    float getFloatIfHasFloat(std::string tag, float defaultValue);
    int getIntIfHasInt(std::string tag, int defaultValue);
    std::string getStringIfHasString(std::string tag, std::string defaultValue);

private:
    pugi::xml_document m_doc;

};

} // namespace sim
