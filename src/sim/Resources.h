#pragma once

#include <QString>
#include <QStringList>

namespace mms {

class Resources {

public:
    Resources() = delete;
    static QString getMazesPath();
    static QStringList getFonts();
    static QStringList getMazes();

private:
    static QStringList getFiles(QString path);

};

} 
