#include "MouseAlgoUtilities.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QProcess>

#include "Directory.h"
#include "SimUtilities.h"

namespace mms {

void MouseAlgoUtilities::execute(const QString& mouseAlgorithm) {

    // Check to see if there is some directory with the given name
    QString selectedMouseAlgo(mouseAlgorithm);
    if (!MouseAlgoUtilities::getMouseAlgos().contains(selectedMouseAlgo)) {
         qCritical().noquote().nospace()
            << "\"" << mouseAlgorithm << "\" is not a valid maze"
            << " algorithm.";
         SimUtilities::quit();
    }

    // Get the maze algo directory
    QString selectedMouseAlgoPath = 
        Directory::get()->getSrcMouseAlgosDirectory() + mouseAlgorithm;

    // Get the files for the algorithm
    QPair<QStringList, QStringList> files =
        MouseAlgoUtilities::getFiles(selectedMouseAlgoPath);
    QStringList relativePaths = files.first;
    QStringList absolutePaths = files.second;

    // TODO: MACK - make these constants, dedup some of this
    if (relativePaths.contains(QString("Main.cpp"))) {

        QString binPath = selectedMouseAlgoPath + "/a.out";

        // Build
        QStringList buildArgs = absolutePaths.filter(".cpp");
        buildArgs << "-o";
        buildArgs << binPath;
        QProcess buildProcess;
        buildProcess.start("g++", buildArgs);
        buildProcess.waitForFinished();
        if (buildProcess.exitCode() != 0) {
            qCritical().noquote()
                << "Failed to build mouse algo!"
                << "\n\n" + buildProcess.readAllStandardError();
            SimUtilities::quit();
        }

        // Run
        QProcess runProcess;
        runProcess.start(binPath);
        // TODO: MACK - can't wait for finished here
        runProcess.waitForFinished();
        if (runProcess.exitCode() != 0) {
            qCritical().noquote()
                << "Mouse algo crashed!"
                << "\n\n" + runProcess.readAllStandardError();
            SimUtilities::quit();
        }

        // Success
        return;
    } 

    // Invalid files
    qCritical().noquote().nospace()
        << "No \"Main\" file found in \""
        << selectedMouseAlgoPath << "\"";
    SimUtilities::quit();
}
        

// TODO: MACK - move this to utils, reuse for mouse and maze algos
QStringList MouseAlgoUtilities::getMouseAlgos() {
    QDir mouseAlgosDir(Directory::get()->getSrcMouseAlgosDirectory());
    mouseAlgosDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    mouseAlgosDir.setSorting(QDir::Name | QDir::QDir::IgnoreCase);
    QStringList algos = mouseAlgosDir.entryList();
    return algos;
}

// TODO: MACK - move this to utils
QPair<QStringList, QStringList> MouseAlgoUtilities::getFiles(const QString& dirPath) {
    // Get all files in the directory
    QDir dir(dirPath);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QDirIterator iterator(dir, QDirIterator::Subdirectories);
    QStringList relativePaths;
    QStringList absolutePaths;
    while (iterator.hasNext()) {
        iterator.next();
        relativePaths << iterator.fileName();
        absolutePaths << iterator.filePath();
    }
    return {relativePaths, absolutePaths};
}

} //namespace mms
