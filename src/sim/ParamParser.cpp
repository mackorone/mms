#include "ParamParser.h"

#include "Assert.h"
#include "Color.h"
#include "LayoutType.h"
#include "Logging.h"
#include "MazeFileType.h"
#include "SimUtilities.h"
#include "TileTextAlignment.h"

namespace sim {

const QString ParamParser::PARAMETERS_TAG = "parameters";

ParamParser::ParamParser(const QString& filePath) {
    m_fileIsReadable = m_doc.load_file(filePath.toStdString().c_str());
    if (!m_fileIsReadable) {
        Logging::get()->warn(
            "Unable to read parameters from \"%v\": %v. "
            "Using default values for all parameters.",
            filePath.toStdString(), m_fileIsReadable.description());
    }
    m_root = m_doc.child(PARAMETERS_TAG.toStdString().c_str());
}

bool ParamParser::hasBoolValue(const QString& tag){
    return SimUtilities::isBool(m_root.child(tag.toStdString().c_str()).child_value());
}

bool ParamParser::hasDoubleValue(const QString& tag){
    return SimUtilities::isDouble(m_root.child(tag.toStdString().c_str()).child_value());
}

bool ParamParser::hasIntValue(const QString& tag){
    return SimUtilities::isInt(m_root.child(tag.toStdString().c_str()).child_value());
}

bool ParamParser::hasCharValue(const QString& tag){
    return hasStringValue(tag) && getStringValue(tag).size() == 1;
}

bool ParamParser::hasStringValue(const QString& tag){
    return (!QString(m_root.child(tag.toStdString().c_str()).child_value()).isEmpty());
}

bool ParamParser::getBoolValue(const QString& tag) {
    return SimUtilities::strToBool(m_root.child(tag.toStdString().c_str()).child_value());
}

double ParamParser::getDoubleValue(const QString& tag) {
    return SimUtilities::strToDouble(m_root.child(tag.toStdString().c_str()).child_value());
}

int ParamParser::getIntValue(const QString& tag) {
    return SimUtilities::strToInt(m_root.child(tag.toStdString().c_str()).child_value());
}

char ParamParser::getCharValue(const QString& tag) {
    return QString(m_root.child(tag.toStdString().c_str()).child_value()).at(0).toLatin1();
}

QString ParamParser::getStringValue(const QString& tag) {
    return QString(m_root.child(tag.toStdString().c_str()).child_value());
}

bool ParamParser::getBoolIfHasBool(const QString& tag, bool defaultValue) {
    if (!hasBoolValue(tag)) {
        printTagNotFound("bool", tag, (defaultValue ? "true" : "false"));
        return defaultValue;
    }
    return getBoolValue(tag);
}

double ParamParser::getDoubleIfHasDouble(const QString& tag, double defaultValue) {
    if (!hasDoubleValue(tag)) {
        printTagNotFound("double", tag, QString::number(defaultValue));
        return defaultValue;
    }
    return getDoubleValue(tag);
}

int ParamParser::getIntIfHasInt(const QString& tag, int defaultValue) {
    if (!hasIntValue(tag)) {
        printTagNotFound("int", tag, QString::number(defaultValue));
        return defaultValue;
    }
    return getIntValue(tag);
}

char ParamParser::getCharIfHasChar(const QString& tag, char defaultValue) {
    if (!hasCharValue(tag)) {
        printTagNotFound("char", tag, QString("\"") + defaultValue + QString("\""));
        return defaultValue;
    }
    return getCharValue(tag);
}

QString ParamParser::getStringIfHasString(const QString& tag, const QString& defaultValue) {
    if (!hasStringValue(tag)) {
        printTagNotFound("string", tag, QString("\"") + defaultValue + QString("\""));
        return defaultValue;
    }
    return getStringValue(tag);
}

double ParamParser::getDoubleIfHasDoubleAndNotLessThan(const QString& tag, double defaultValue, double min) {
    return getNumIfHasNumAndInRange("double", tag, defaultValue, min, std::numeric_limits<double>::max());
}

double ParamParser::getDoubleIfHasDoubleAndNotGreaterThan(const QString& tag, double defaultValue, double max) {
    return getNumIfHasNumAndInRange("double", tag, defaultValue, std::numeric_limits<double>::min(), max);
}

double ParamParser::getDoubleIfHasDoubleAndInRange(const QString& tag, double defaultValue, double min, double max) {
    return getNumIfHasNumAndInRange("double", tag, defaultValue, min, max);
}

int ParamParser::getIntIfHasIntAndNotLessThan(const QString& tag, int defaultValue, int min) {
    return getNumIfHasNumAndInRange("int", tag, defaultValue, min, std::numeric_limits<int>::max());
}

int ParamParser::getIntIfHasIntAndNotGreaterThan(const QString& tag, int defaultValue, int max) {
    return getNumIfHasNumAndInRange("int", tag, defaultValue, std::numeric_limits<int>::min(), max);
}

int ParamParser::getIntIfHasIntAndInRange(const QString& tag, int defaultValue, int min, int max) {
    return getNumIfHasNumAndInRange("int", tag, defaultValue, min, max);
}

QString ParamParser::getStringIfHasStringAndIsColor(const QString& tag, const QString& defaultValue) {
    return getStringIfHasStringAndIsSpecial("color", tag, defaultValue, STRING_TO_COLOR);
}

QString ParamParser::getStringIfHasStringAndIsDirection(const QString& tag, const QString& defaultValue) {
    return getStringIfHasStringAndIsSpecial("direction", tag, defaultValue, STRING_TO_DIRECTION);
}

QString ParamParser::getStringIfHasStringAndIsLayoutType(const QString& tag, const QString& defaultValue) {
    return getStringIfHasStringAndIsSpecial("layout type", tag, defaultValue, STRING_TO_LAYOUT_TYPE);
}

QString ParamParser::getStringIfHasStringAndIsMazeFileType(const QString& tag, const QString& defaultValue) {
    return getStringIfHasStringAndIsSpecial("maze file type", tag, defaultValue, STRING_TO_MAZE_FILE_TYPE);
}

QString ParamParser::getStringIfHasStringAndIsTileTextAlignment(const QString& tag, const QString& defaultValue) {
    return getStringIfHasStringAndIsSpecial("tile text alignment", tag, defaultValue, STRING_TO_TILE_TEXT_ALIGNMENT);
}

void ParamParser::printTagNotFound(const QString& type, const QString& tag, const QString& defaultValue) {
    if (m_fileIsReadable) {
        Logging::get()->warn(
            "Could not find %v parameter \"%v\". Using default value of %v.",
            type.toStdString(), tag.toStdString(), defaultValue.toStdString());
    }
}

void ParamParser::printLessThan(const QString& type, const QString& tag, const QString& value,
    const QString& defaultValue, const QString& min) {
    Logging::get()->warn(
        "The value of the %v parameter \"%v\" is %v and is less than "
        "the minimum allowed value of %v. Using default value of %v.",
        type.toStdString(), tag.toStdString(), value.toStdString(), min.toStdString(), defaultValue.toStdString());
}

void ParamParser::printGreaterThan(const QString& type, const QString& tag, const QString& value,
    const QString& defaultValue, const QString& max) {
    Logging::get()->warn(
        "The value of the %v parameter \"%v\" is %v and is greater than "
        "the maximum allowed value of %v. Using default value of %v.",
        type.toStdString(), tag.toStdString(), value.toStdString(), max.toStdString(), defaultValue.toStdString());	
}

void ParamParser::printNotSpecialString(
        const QString& type,
        const QString& tag,
        const QString& value,
        const QString& defaultValue) {
    Logging::get()->warn(
        "The value of string parameter \"%v\" is \"%v\" and is "
        "not a valid %v. Using default value of \"%v\".",
        tag.toStdString(), value.toStdString(), type.toStdString(), defaultValue.toStdString());
}

}; // namespace sim
