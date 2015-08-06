#include "ParamParser.h"

#include "Assert.h"
#include "Colors.h"
#include "Logging.h"
#include "SimUtilities.h"

namespace sim {

ParamParser::ParamParser(const std::string& filePath) {
    // Open the document
    m_result = m_doc.load_file(filePath.c_str());
    if (!m_result) {
            /*
        LOG(ERROR) << "Unable to read parameters from \"" << filePath  << "\": "
            << m_result.description() << ". Using default values for all parameters.";
            */
    }
}

bool ParamParser::hasBoolValue(const std::string& tag){
    return SimUtilities::isBool(m_doc.child(tag.c_str()).child_value());
}

bool ParamParser::hasDoubleValue(const std::string& tag){
    return SimUtilities::isDouble(m_doc.child(tag.c_str()).child_value());
}

bool ParamParser::hasIntValue(const std::string& tag){
    return SimUtilities::isInt(m_doc.child(tag.c_str()).child_value());
}

bool ParamParser::hasStringValue(const std::string& tag){
    return (!std::string(m_doc.child(tag.c_str()).child_value()).empty());
}

bool ParamParser::getBoolValue(const std::string& tag) {
    return SimUtilities::strToBool(m_doc.child(tag.c_str()).child_value());
}

double ParamParser::getDoubleValue(const std::string& tag) {
    return SimUtilities::strToDouble(m_doc.child(tag.c_str()).child_value());
}

int ParamParser::getIntValue(const std::string& tag) {
    return SimUtilities::strToInt(m_doc.child(tag.c_str()).child_value());
}

std::string ParamParser::getStringValue(const std::string& tag) {
    return std::string(m_doc.child(tag.c_str()).child_value());
}

bool ParamParser::getBoolIfHasBool(const std::string& tag, bool defaultValue) {
    if (!hasBoolValue(tag)) {
        if (m_result) {
            /*
            LOG(WARN) << "Could not find bool parameter \"" << tag << "\". Using default value of "
                << (defaultValue ? "true" : "false") << " for \""  << tag << "\".";
            */
        }
        return defaultValue;
    }
    return getBoolValue(tag);
}

double ParamParser::getDoubleIfHasDouble(const std::string& tag, double defaultValue) {
    if (!hasDoubleValue(tag)) {
        if (m_result) {
            /*
            LOG(WARN) << "Could not find double parameter \"" << tag
                << "\". Using default value of " << defaultValue << " for \"" << tag << "\".";
            */
        }
        return defaultValue;
    }
    return getDoubleValue(tag);
}

int ParamParser::getIntIfHasInt(const std::string& tag, int defaultValue) {
    if (!hasIntValue(tag)) {
        if (m_result) {
            /*
            LOG(WARN) << "Could not find int parameter \"" << tag
                << "\". Using default value of " << defaultValue << " for \"" << tag << "\".";
            */
        }
        return defaultValue;
    }
    return getIntValue(tag);
}

std::string ParamParser::getStringIfHasString(const std::string& tag, const std::string& defaultValue) {
    if (!hasStringValue(tag)) {
        if (m_result) {
            /*
            LOG(WARN) << "Could not find string parameter \"" << tag
                << "\". Using default value of \"" << defaultValue << "\" for \"" << tag << "\".";
            */
        }
        return defaultValue;
    }
    return getStringValue(tag);
}

double ParamParser::getDoubleIfHasDoubleAndInRange(const std::string& tag, double defaultValue, double min, double max) {
    if (!hasDoubleValue(tag)) {
        if (m_result) {
            /*
            LOG(WARN) << "Could not find double parameter \"" << tag
                << "\". Using default value of " << defaultValue << " for \""  << tag << "\".";
            */
        }
        return defaultValue;
    }
    double value = getDoubleValue(tag);
    if (value < min || max < value) {
            /*
        LOG(WARN) << "The value of the double parameter \"" << tag << "\" is " << value
            << " and not in the valid range of [" << min << ", " << max
            << "]. Using default value of " << defaultValue << " for \""  << tag << "\".";
            */
        return defaultValue;
    }
    return value;
}

int ParamParser::getIntIfHasIntAndInRange(const std::string& tag, int defaultValue, int min, int max) {
    if (!hasIntValue(tag)) {
        if (m_result) {
            /*
            LOG(WARN) << "Could not find int parameter \"" << tag
                << "\". Using default value of " << defaultValue << " for \"" << tag << "\".";
            */
        }
        return defaultValue;
    }
    int value = getIntValue(tag);
    if (value < min || max < value) {
            /*
        LOG(WARN) << "The value of the int parameter \"" << tag << "\" is " << value
            << " and not in the valid range of [" << min << ", " << max
            << "]. Using default value of " << defaultValue << " for \"" << tag << "\".";
            */
        return defaultValue;
    }
    return value;
}

std::string ParamParser::getStringIfHasStringAndIsColor(const std::string& tag, const std::string& defaultValue) {
    if (!hasStringValue(tag)) {
        if (m_result) {
            /*
            LOG(WARN) << "Could not find string parameter \"" << tag
                << "\". Using default value of \"" << defaultValue << "\" for \"" << tag << "\".";
            */
        }
        return defaultValue;
    }
    std::string value = getStringValue(tag);
    if (COLOR_STRINGS.find(value) == COLOR_STRINGS.end()) {
            /*
        LOG(WARN) << "The value of string parameter \"" << tag << "\" is \"" << value
            << "\" and is not a valid color. Using default value of \"" << defaultValue
            << "\" for \"" << tag << "\".";
            */
        return defaultValue;
    }
    return value;
}

std::string ParamParser::getStringIfHasStringAndIsDirection(const std::string& tag, const std::string& defaultValue) {
    if (!hasStringValue(tag)) {
        if (m_result) {
            /*
            LOG(WARN) << "Could not find string parameter \"" << tag
                << "\". Using default value of \"" << defaultValue << "\" for \"" << tag << "\".";
            */
        }
        return defaultValue;
    }
    std::string value = getStringValue(tag);
    if (DIRECTION_STRINGS.find(value) == DIRECTION_STRINGS.end()) {
            /*
        LOG(WARN) << "The value of string parameter \"" << tag << "\" is \"" << value
            << "\" and is not a valid direction. Using default value of \"" << defaultValue
            << "\" for \"" << tag << "\".";
            */
        return defaultValue;
    }
    return value;
}

}; // namespace sim
