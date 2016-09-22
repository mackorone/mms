#pragma once

#include <QPair>
#include <QString>
#include <QStringList>

namespace mms {

class MouseAlgoUtilities {

public:
    MouseAlgoUtilities() = delete;
    static void execute(const QString& mouseAlgorithm);

private:
    static QStringList getMouseAlgos();
    static QPair<QStringList, QStringList> getFiles(const QString& dirPath);
 

};

} // namespace mms
