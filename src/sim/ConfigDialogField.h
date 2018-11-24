#pragma once

#include <QString>
#include <QVariant>

namespace mms {

enum class ConfigDialogFieldType {
    DIRECTORY,
    FILE,
    STRING,
};

struct ConfigDialogField {
    QString label;
    ConfigDialogFieldType type;
    QVariant initialLineEditValue;
    bool allowEmptyLineEditValue = true;
}; 

} 
