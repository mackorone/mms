#include "ParamParser.h"

#include <iostream>

#include "Assert.h"
#include "SimUtilities.h"

namespace sim {

ParamParser::ParamParser(std::string filePath) {
    // Open the document
    pugi::xml_parse_result result = m_doc.load_file(filePath.c_str());
    if (!result) {
        SimUtilities::print("Error: Unable to read parameters from \"" + filePath + "\": " + result.description());
        SimUtilities::quit();
    }
}

bool ParamParser::hasBoolValue(std::string tag){
    return SimUtilities::isBool(m_doc.child(tag.c_str()).child_value());
}

bool ParamParser::hasFloatValue(std::string tag){
    return SimUtilities::isFloat(m_doc.child(tag.c_str()).child_value());
}

bool ParamParser::hasIntValue(std::string tag){
    return SimUtilities::isInt(m_doc.child(tag.c_str()).child_value());
}

bool ParamParser::hasStringValue(std::string tag){
    return (!std::string(m_doc.child(tag.c_str()).child_value()).empty());
}

bool ParamParser::getBoolValue(std::string tag) {
    return SimUtilities::strToBool(m_doc.child(tag.c_str()).child_value());
}

float ParamParser::getFloatValue(std::string tag) {
    return SimUtilities::strToFloat(m_doc.child(tag.c_str()).child_value());
}

int ParamParser::getIntValue(std::string tag) {
    return SimUtilities::strToInt(m_doc.child(tag.c_str()).child_value());
}

std::string ParamParser::getStringValue(std::string tag) {
    return std::string(m_doc.child(tag.c_str()).child_value());
}

bool ParamParser::getBoolIfHasBool(std::string tag, bool defaultValue) {
    return (hasBoolValue(tag) ? getBoolValue(tag) : defaultValue);
}

float ParamParser::getFloatIfHasFloat(std::string tag, float defaultValue) {
    return (hasFloatValue(tag) ? getFloatValue(tag) : defaultValue);
}

int ParamParser::getIntIfHasInt(std::string tag, int defaultValue) {
    return (hasIntValue(tag) ? getIntValue(tag) : defaultValue);
}

std::string ParamParser::getStringIfHasString(std::string tag, std::string defaultValue) {
    return (hasStringValue(tag) ? getStringValue(tag) : defaultValue);
}

}; // namespace sim
