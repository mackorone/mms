#include "Resources.h"

#include <QDir>

namespace mms {

QStringList Resources::getFonts() {
    return getFiles(":/resources/fonts/");
}

QStringList Resources::getMazes() {
    return getFiles(":/resources/mazes/");
}

QStringList Resources::getMice() {
    return getFiles(":/resources/mice/");
}

QStringList Resources::getFiles(QString path) {
    QStringList files;
    for (const auto& info : QDir(path).entryInfoList()) {
        files.append(info.absoluteFilePath());
    }
    return files;
}

} // namespace mms
