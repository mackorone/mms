#include "ParamParser.h"

#include "Assert.h"
#include "SimUtilities.h"

namespace sim {

ParamParser::ParamParser(std::string filePath) {
    // Open the document
    pugi::xml_parse_result result = m_doc.load_file(filePath.c_str());
    if (!result) {
        // This is a special case where we have to use dump(). We can't use print()
        // because print() uses some parameters that we haven't yet initialized.
        SimUtilities::dump("Error: Unable to read parameters from \"" + filePath + "\": "
            + result.description() + ".\nUsing default values for all parameters instead.");
    }
}

bool ParamParser::hasBoolValue(std::string tag){
    return SimUtilities::isBool(m_doc.child(tag.c_str()).child_value());
}

bool ParamParser::hasDoubleValue(std::string tag){
    return SimUtilities::isDouble(m_doc.child(tag.c_str()).child_value());
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

double ParamParser::getDoubleValue(std::string tag) {
    return SimUtilities::strToDouble(m_doc.child(tag.c_str()).child_value());
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

double ParamParser::getDoubleIfHasDouble(std::string tag, double defaultValue) {
    return (hasDoubleValue(tag) ? getDoubleValue(tag) : defaultValue);
}

int ParamParser::getIntIfHasInt(std::string tag, int defaultValue) {
    return (hasIntValue(tag) ? getIntValue(tag) : defaultValue);
}

std::string ParamParser::getStringIfHasString(std::string tag, std::string defaultValue) {
    return (hasStringValue(tag) ? getStringValue(tag) : defaultValue);
}

}; // namespace sim
