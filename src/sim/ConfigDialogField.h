#pragma once

#include <QString>
#include <QVariant>
#include <QVector>

namespace mms {

struct ConfigDialogField {

    QString key;
    QString label;
    QVariant initialValue;

    QVector<QVariant> filterValues = {};
    bool inclusiveFilter = false;

    bool fileBrowser = false;
    bool onlyDirectories = false;
}; 

} // namespace mms
