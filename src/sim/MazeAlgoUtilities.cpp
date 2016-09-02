#include "MazeAlgoUtilities.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QProcess>

#include "Directory.h"
#include "MazeFileUtilities.h"
#include "SimUtilities.h"

namespace mms {

BasicMaze MazeAlgoUtilities::generate(
        const QString& mazeAlgorithm,
        int width,
        int height) {

    // Check to see if there is some directory with the given name
    QString selectedMazeAlgo(mazeAlgorithm);
    if (!MazeAlgoUtilities::getMazeAlgos().contains(selectedMazeAlgo)) {
         qCritical().noquote().nospace()
            << "\"" << mazeAlgorithm << "\" is not a valid maze"
            << " algorithm.";
         SimUtilities::quit();
    }

    // Get the maze algo directory
    QString selectedMazeAlgoPath = 
        Directory::get()->getSrcMazeAlgosDirectory() + mazeAlgorithm;

    // Get the files for the algorithm
    QPair<QStringList, QStringList> files =
        MazeAlgoUtilities::getFiles(selectedMazeAlgoPath);
    QStringList relativePaths = files.first;
    QStringList absolutePaths = files.second;
    
    // TODO: MACK - make these constants, dedup some of this
    if (relativePaths.contains(QString("Main.cpp"))) {

        QString binPath = selectedMazeAlgoPath + "/a.out";

        // Build
        QStringList buildArgs = absolutePaths.filter(".cpp");
        buildArgs << "-o";
        buildArgs << binPath;
        QProcess buildProcess;
        buildProcess.start("g++", buildArgs);
        buildProcess.waitForFinished();
        if (buildProcess.exitCode() != 0) {
            qCritical().noquote()
                << "Failed to build maze algo!"
                << "\n\n" + buildProcess.readAllStandardError();
            SimUtilities::quit();
        }

        // Run
        QStringList runArgs;
        runArgs << QString::number(width);
        runArgs << QString::number(height);
        QProcess runProcess;
        runProcess.start(binPath, runArgs);
        runProcess.waitForFinished();
        if (runProcess.exitCode() != 0) {
            qCritical().noquote()
                << "Failed to run maze algo!"
                << "\n\n" + runProcess.readAllStandardError();
            SimUtilities::quit();
        }
        QByteArray bytes = runProcess.readAllStandardError();

        return MazeFileUtilities::loadBytes(bytes);
    }
    else if (relativePaths.contains(QString("Main.py"))) {

        QStringList args;
        args << selectedMazeAlgoPath + QString("/Main.py");
        args << QString::number(width);
        args << QString::number(height);
        QProcess process;
        process.start("python", args);
        process.waitForFinished();
        if (process.exitCode() != 0) {
            qCritical().noquote()
                << "Failed to run maze algo!"
                << "\n\n" + process.readAllStandardError();
            SimUtilities::quit();
        }
        QByteArray bytes = process.readAllStandardError();

        return MazeFileUtilities::loadBytes(bytes);
    }

    // Invalid files
    qCritical().noquote().nospace()
        << "No \"Main\" file found in \""
        << selectedMazeAlgoPath << "\"";
    SimUtilities::quit();
}

QStringList MazeAlgoUtilities::getMazeAlgos() {
    QDir mazeAlgosDir(Directory::get()->getSrcMazeAlgosDirectory());
    mazeAlgosDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    mazeAlgosDir.setSorting(QDir::Name | QDir::QDir::IgnoreCase);
    QStringList algos = mazeAlgosDir.entryList();
    return algos;
}

// TODO: MACK - move this to utils
QPair<QStringList, QStringList> MazeAlgoUtilities::getFiles(const QString& dirPath) {
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
