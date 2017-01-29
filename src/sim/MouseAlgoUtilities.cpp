#include "MouseAlgoUtilities.h"

#include <QProcess>

#include "Directory.h"
#include "SimUtilities.h"

namespace mms {

// TODO: MACK - dedup with maze algorithm as much as possible
void MouseAlgoUtilities::build(const QString& mouseAlgorithm) {

    // Check to see if there is some directory with the given name
    QString mouseAlgosDir(Directory::get()->getSrcMouseAlgosDirectory());
    if (!SimUtilities::getTopLevelDirs(mouseAlgosDir).contains(mouseAlgorithm)) {
         qCritical().noquote().nospace()
            << "\"" << mouseAlgorithm << "\" is not a valid mouse"
            << " algorithm.";
         SimUtilities::quit();
    }

    // Get the mouse algo directory
    QString selectedMouseAlgoPath = 
        Directory::get()->getSrcMouseAlgosDirectory() + mouseAlgorithm;

    // Get the files for the algorithm
    QPair<QStringList, QStringList> files =
        SimUtilities::getFiles(selectedMouseAlgoPath);
    QStringList relativePaths = files.first;
    QStringList absolutePaths = files.second;

    // TODO: MACK - make these constants, dedup some of this
    if (relativePaths.contains(QString("Main.cpp"))) {

        // Build
        QStringList buildArgs = absolutePaths.filter(".cpp");
        buildArgs << "-W";
        buildArgs << "-g";
        buildArgs << "-o";
        buildArgs << selectedMouseAlgoPath + "/a.out";
        QProcess buildProcess;
        /* TODO: MACK */
        connect(
            &buildProcess,
            &QProcess::readyReadStandardError,
            this,
            [&](){
                emit buildError(buildProcess.readAllStandardError());
            }
        );
        buildProcess.start("g++", buildArgs);
        buildProcess.waitForFinished();
        // TODO: MACK
        if (buildProcess.exitCode() != 0) {
            qCritical().noquote()
                << "Failed to build mouse algo!"
                << "\n\n" + buildProcess.readAllStandardError();
            SimUtilities::quit();
        }
    }
}

} //namespace mms
