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
    static QStringList getMice();

private:

    static QStringList getFiles(QString path);

};

} // namespace mms
