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

    // Get the files for the algorithm
    QPair<QStringList, QStringList> files = MazeAlgoUtilities::getFiles(mazeAlgorithm);
    QStringList relativePaths = files.first;
    QStringList absolutePaths = files.second;
    
    // TODO: MACK - dedup from getFiles
    QDir mazeAlgosDir(Directory::get()->getSrcMazeAlgosDirectory());
    QDir selectedMazeAlgoDir(mazeAlgosDir);
    selectedMazeAlgoDir.cd(mazeAlgorithm);

    // Deduce whether or not it's a C++ or Python algo
    QStringList args;
    if (relativePaths.contains(QString("Main.cpp"))) {
        // TODO: MACK args
        SimUtilities::quit();
    }
    else if (relativePaths.contains(QString("Main.py"))) {
        args << selectedMazeAlgoDir.absolutePath() + QString("/Main.py");
        args << QString::number(width);
        args << QString::number(height);
    }
    else {
        qCritical().noquote().nospace()
            << "No \"Main.{py,cpp}\" found in \""
            << selectedMazeAlgoDir.absolutePath() << "\"";
        SimUtilities::quit();
    }

    QProcess process; // TODO: MACK - pass in parent here
    process.start("python", args);
    process.waitForFinished();
    QByteArray bytes = process.readAll();
    return MazeFileUtilities::loadBytes(bytes);
}

QStringList MazeAlgoUtilities::getMazeAlgos() {
    QDir mazeAlgosDir(Directory::get()->getSrcMazeAlgosDirectory());
    mazeAlgosDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    mazeAlgosDir.setSorting(QDir::Name | QDir::QDir::IgnoreCase);
    QStringList algos = mazeAlgosDir.entryList();
    return algos;
}

QPair<QStringList, QStringList> MazeAlgoUtilities::getFiles(const QString& mazeAlgorithm) {
    // Get all files in the mazeAlgo directory, recursively
    QDir mazeAlgosDir(Directory::get()->getSrcMazeAlgosDirectory());
    QDir selectedMazeAlgoDir(mazeAlgosDir);
    selectedMazeAlgoDir.cd(mazeAlgorithm);
    selectedMazeAlgoDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QDirIterator iterator(selectedMazeAlgoDir, QDirIterator::Subdirectories);
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
