#include "Resources.h"

#include <QDir>

namespace mms {

QString Resources::getMazesPath() {
    return ":/resources/mazes/";
}

QStringList Resources::getFonts() {
    return getFiles(":/resources/fonts/");
}

QStringList Resources::getMazes() {
    return getFiles(getMazesPath());
}

QStringList Resources::getFiles(QString path) {
    QStringList files;
    for (const auto& info : QDir(path).entryInfoList()) {
        files.append(info.absoluteFilePath());
    }
    return files;
}

} 
