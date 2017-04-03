#pragma once

#include <QCheckBox>
#include <QObject>
#include <QPushButton>
#include <QProcess>
#include <QString>
#include <QStringList>

#include "TextDisplay.h"

namespace mms {

class ProcessUtilities {

public:

    ProcessUtilities() = delete;

    static bool start(
        const QString& command,
        const QString& directory,
        QProcess* process);

    static void build(
        const QString& name,
        const QStringList& names,
        const QString& buildCommand,
        const QString& directory,
        QCheckBox* autoClear,
        TextDisplay* output,
        QPushButton* button,
        QObject* parent);

};

} // namespace mms
