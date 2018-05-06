#include "ParamParser.h"

#include "Assert.h"
#include "Color.h"
#include "ConfigDialog.h"
#include "ConfigDialogField.h"
#include "Direction.h"
#include "LayoutType.h"
#include "Logging.h"
#include "MazeFileType.h"
#include "Settings.h"
#include "SimUtilities.h"

namespace mms {

const QString ParamParser::GROUP = "miscellaneous";

// TODO: MACK

void ParamParser::execEditDialog() {

    /*
    ConfigDialogField windowWidthField;
    windowWidthField.key = WINDOW_WIDTH_KEY;
    windowWidthField.label = "Window Width";
    windowWidthField.initialValue = getValue(WINDOW_WIDTH_KEY);

    // Execute the dialog
    ConfigDialog dialog(
        "Edit",
        "Miscellaneous",
        {
            windowWidthField,
        },
        false
    );

    // Cancel was pressed
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

    // Ok was pressed
    for (const QString& key : {
        WINDOW_WIDTH_KEY,
    }) {
        Settings::get()->update(GROUP, key, dialog.getValue(key));
    }
    */
}

QString ParamParser::getValue(const QString& key) {
    return Settings::get()->value(GROUP, key);
}

void ParamParser::setValue(const QString& key, const QString& value) {
    Settings::get()->update(GROUP, key, value);
}

// TODO: MACK

// TODO: MACK - rename tag to key
bool ParamParser::hasBoolValue(const QString& tag){
    return SimUtilities::isBool(getValue(tag));
}

bool ParamParser::hasDoubleValue(const QString& tag){
    return SimUtilities::isDouble(getValue(tag));
}

bool ParamParser::hasIntValue(const QString& tag){
    return SimUtilities::isInt(getValue(tag));
}

bool ParamParser::hasCharValue(const QString& tag){
    return hasStringValue(tag) && getStringValue(tag).size() == 1;
}

bool ParamParser::hasStringValue(const QString& tag){
    return !getValue(tag).isEmpty();
}

bool ParamParser::getBoolValue(const QString& tag) {
    return SimUtilities::strToBool(getValue(tag));
}

double ParamParser::getDoubleValue(const QString& tag) {
    return SimUtilities::strToDouble(getValue(tag));
}

int ParamParser::getIntValue(const QString& tag) {
    return SimUtilities::strToInt(getValue(tag));
}

char ParamParser::getCharValue(const QString& tag) {
    return getValue(tag).at(0).toLatin1();
}

QString ParamParser::getStringValue(const QString& tag) {
    return getValue(tag);
}

bool ParamParser::getBoolIfHasBool(const QString& tag, bool defaultValue) {
    if (!hasBoolValue(tag)) {
        printTagNotFound("bool", tag, (defaultValue ? "true" : "false"));
        setValue(tag, (defaultValue ? "true" : "false"));
        return defaultValue;
    }
    return getBoolValue(tag);
}

double ParamParser::getDoubleIfHasDouble(const QString& tag, double defaultValue) {
    if (!hasDoubleValue(tag)) {
        printTagNotFound("double", tag, QString::number(defaultValue));
        setValue(tag, QString::number(defaultValue));
        return defaultValue;
    }
    return getDoubleValue(tag);
}

int ParamParser::getIntIfHasInt(const QString& tag, int defaultValue) {
    if (!hasIntValue(tag)) {
        printTagNotFound("int", tag, QString::number(defaultValue));
        setValue(tag, QString::number(defaultValue));
        return defaultValue;
    }
    return getIntValue(tag);
}

char ParamParser::getCharIfHasChar(const QString& tag, char defaultValue) {
    if (!hasCharValue(tag)) {
        printTagNotFound("char", tag, QString("\"") + defaultValue + QString("\""));
        setValue(tag, QChar(defaultValue));
        return defaultValue;
    }
    return getCharValue(tag);
}

QString ParamParser::getStringIfHasString(const QString& tag, const QString& defaultValue) {
    if (!hasStringValue(tag)) {
        printTagNotFound("string", tag, QString("\"") + defaultValue + QString("\""));
        setValue(tag, defaultValue);
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
    return getStringIfHasStringAndIsSpecial("color", tag, defaultValue, STRING_TO_COLOR());
}

QString ParamParser::getStringIfHasStringAndIsDirection(const QString& tag, const QString& defaultValue) {
    return getStringIfHasStringAndIsSpecial("direction", tag, defaultValue, STRING_TO_DIRECTION());
}

QString ParamParser::getStringIfHasStringAndIsLayoutType(const QString& tag, const QString& defaultValue) {
    return getStringIfHasStringAndIsSpecial("layout type", tag, defaultValue, STRING_TO_LAYOUT_TYPE());
}

QString ParamParser::getStringIfHasStringAndIsMazeFileType(const QString& tag, const QString& defaultValue) {
    return getStringIfHasStringAndIsSpecial("maze file type", tag, defaultValue, STRING_TO_MAZE_FILE_TYPE());
}

void ParamParser::printTagNotFound(const QString& type, const QString& tag, const QString& defaultValue) {
    // TODO: MACK
    /*
    if (m_fileIsReadable) {
    */
    qWarning().noquote().nospace()
        << "Could not find " << type << " parameter \"" << tag << "\"."
        << " Using default value of " << defaultValue << ".";
}

void ParamParser::printLessThan(const QString& type, const QString& tag, const QString& value,
    const QString& defaultValue, const QString& min) {
    qWarning().noquote().nospace()
        << "The value of the " << type << " parameter \"" << tag << "\" is "
        << value << " and is less than the minimum allowed value of " << min
        << ". Using default value of " << defaultValue << ".";
}

void ParamParser::printGreaterThan(const QString& type, const QString& tag, const QString& value,
    const QString& defaultValue, const QString& max) {
    qWarning().noquote().nospace()
        << "The value of the " << type << " parameter \"" << tag << "\" is "
        << value << " and is greater than the maximum allowed value of " << max
        << ". Using default value of " << defaultValue    << ".";
}

void ParamParser::printNotSpecialString(
        const QString& type,
        const QString& tag,
        const QString& value,
        const QString& defaultValue) {
    qWarning().noquote().nospace()
        << "The value of string parameter \"" << tag << "\" is \"" << value
        << "\" and is not a valid " << type << ". Using default value of \""
        << defaultValue << "\".";
}

} // namespace mms
