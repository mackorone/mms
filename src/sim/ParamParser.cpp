#include "ParamParser.h"

#include "Assert.h"
#include "Color.h"
#include "LayoutType.h"
#include "Logging.h"
#include "MazeFileType.h"
#include "SimUtilities.h"
#include "TileTextAlignment.h"

namespace sim {

const std::string ParamParser::PARAMETERS_TAG = "parameters";

ParamParser::ParamParser(const std::string& filePath) {
    m_fileIsReadable = m_doc.load_file(filePath.c_str());
    if (!m_fileIsReadable) {
        L()->warn(
            "Unable to read parameters from \"%v\": %v. "
            "Using default values for all parameters.",
            filePath, m_fileIsReadable.description());
    }
    m_root = m_doc.child(PARAMETERS_TAG.c_str());
}

bool ParamParser::hasBoolValue(const std::string& tag){
    return SimUtilities::isBool(m_root.child(tag.c_str()).child_value());
}

bool ParamParser::hasDoubleValue(const std::string& tag){
    return SimUtilities::isDouble(m_root.child(tag.c_str()).child_value());
}

bool ParamParser::hasIntValue(const std::string& tag){
    return SimUtilities::isInt(m_root.child(tag.c_str()).child_value());
}

bool ParamParser::hasCharValue(const std::string& tag){
    return hasStringValue(tag) && getStringValue(tag).size() == 1;
}

bool ParamParser::hasStringValue(const std::string& tag){
    return (!std::string(m_root.child(tag.c_str()).child_value()).empty());
}

bool ParamParser::getBoolValue(const std::string& tag) {
    return SimUtilities::strToBool(m_root.child(tag.c_str()).child_value());
}

double ParamParser::getDoubleValue(const std::string& tag) {
    return SimUtilities::strToDouble(m_root.child(tag.c_str()).child_value());
}

int ParamParser::getIntValue(const std::string& tag) {
    return SimUtilities::strToInt(m_root.child(tag.c_str()).child_value());
}

char ParamParser::getCharValue(const std::string& tag) {
    return std::string(m_root.child(tag.c_str()).child_value()).at(0);
}

std::string ParamParser::getStringValue(const std::string& tag) {
    return std::string(m_root.child(tag.c_str()).child_value());
}

bool ParamParser::getBoolIfHasBool(const std::string& tag, bool defaultValue) {
    if (!hasBoolValue(tag)) {
        printTagNotFound("bool", tag, (defaultValue ? "true" : "false"));
        return defaultValue;
    }
    return getBoolValue(tag);
}

double ParamParser::getDoubleIfHasDouble(const std::string& tag, double defaultValue) {
    if (!hasDoubleValue(tag)) {
        printTagNotFound("double", tag, std::to_string(defaultValue));
        return defaultValue;
    }
    return getDoubleValue(tag);
}

int ParamParser::getIntIfHasInt(const std::string& tag, int defaultValue) {
    if (!hasIntValue(tag)) {
        printTagNotFound("int", tag, std::to_string(defaultValue));
        return defaultValue;
    }
    return getIntValue(tag);
}

char ParamParser::getCharIfHasChar(const std::string& tag, char defaultValue) {
    if (!hasCharValue(tag)) {
        printTagNotFound("char", tag, std::string("\"") + defaultValue + std::string("\""));
        return defaultValue;
    }
    return getCharValue(tag);
}

std::string ParamParser::getStringIfHasString(const std::string& tag, const std::string& defaultValue) {
    if (!hasStringValue(tag)) {
        printTagNotFound("string", tag, std::string("\"") + defaultValue + std::string("\""));
        return defaultValue;
    }
    return getStringValue(tag);
}

double ParamParser::getDoubleIfHasDoubleAndNotLessThan(const std::string& tag, double defaultValue, double min) {
    return getNumIfHasNumAndInRange("double", tag, defaultValue, min, std::numeric_limits<double>::max());
}

double ParamParser::getDoubleIfHasDoubleAndNotGreaterThan(const std::string& tag, double defaultValue, double max) {
    return getNumIfHasNumAndInRange("double", tag, defaultValue, std::numeric_limits<double>::min(), max);
}

double ParamParser::getDoubleIfHasDoubleAndInRange(const std::string& tag, double defaultValue, double min, double max) {
    return getNumIfHasNumAndInRange("double", tag, defaultValue, min, max);
}

int ParamParser::getIntIfHasIntAndNotLessThan(const std::string& tag, int defaultValue, int min) {
    return getNumIfHasNumAndInRange("int", tag, defaultValue, min, std::numeric_limits<int>::max());
}

int ParamParser::getIntIfHasIntAndNotGreaterThan(const std::string& tag, int defaultValue, int max) {
    return getNumIfHasNumAndInRange("int", tag, defaultValue, std::numeric_limits<int>::min(), max);
}

int ParamParser::getIntIfHasIntAndInRange(const std::string& tag, int defaultValue, int min, int max) {
    return getNumIfHasNumAndInRange("int", tag, defaultValue, min, max);
}

std::string ParamParser::getStringIfHasStringAndIsColor(const std::string& tag, const std::string& defaultValue) {
    return getStringIfHasStringAndIsSpecial("color", tag, defaultValue, STRING_TO_COLOR);
}

std::string ParamParser::getStringIfHasStringAndIsDirection(const std::string& tag, const std::string& defaultValue) {
    return getStringIfHasStringAndIsSpecial("direction", tag, defaultValue, STRING_TO_DIRECTION);
}

std::string ParamParser::getStringIfHasStringAndIsLayoutType(const std::string& tag, const std::string& defaultValue) {
    return getStringIfHasStringAndIsSpecial("layout type", tag, defaultValue, STRING_TO_LAYOUT_TYPE);
}

std::string ParamParser::getStringIfHasStringAndIsMazeFileType(const std::string& tag, const std::string& defaultValue) {
    return getStringIfHasStringAndIsSpecial("maze file type", tag, defaultValue, STRING_TO_MAZE_FILE_TYPE);
}

std::string ParamParser::getStringIfHasStringAndIsTileTextAlignment(const std::string& tag, const std::string& defaultValue) {
    return getStringIfHasStringAndIsSpecial("tile text alignment", tag, defaultValue, STRING_TO_TILE_TEXT_ALIGNMENT);
}

void ParamParser::printTagNotFound(const std::string& type, const std::string& tag, const std::string& defaultValue) {
    if (m_fileIsReadable) {
        L()->warn(
            "Could not find %v parameter \"%v\". Using default value of %v.",
            type, tag, defaultValue);
    }
}

void ParamParser::printLessThan(const std::string& type, const std::string& tag, const std::string& value,
    const std::string& defaultValue, const std::string& min) {
    L()->warn(
        "The value of the %v parameter \"%v\" is %v and is less than "
        "the minimum allowed value of %v. Using default value of %v.",
        type, tag, value, min, defaultValue);
}

void ParamParser::printGreaterThan(const std::string& type, const std::string& tag, const std::string& value,
    const std::string& defaultValue, const std::string& max) {
    L()->warn(
        "The value of the %v parameter \"%v\" is %v and is greater than "
        "the maximum allowed value of %v. Using default value of %v.",
        type, tag, value, max, defaultValue);	
}

void ParamParser::printNotSpecialString(const std::string& type, const std::string& tag,
    const std::string& value, const std::string& defaultValue) {
    L()->warn(
        "The value of string parameter \"%v\" is \"%v\" and is "
        "not a valid %v. Using default value of \"%v\".",
        tag, value, type, defaultValue);
}

}; // namespace sim
