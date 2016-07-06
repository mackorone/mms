#include "Directory.h"

#include <QCoreApplication>
#include <QFileInfo>

// TODO: MACK
#include <iostream>

// TODO: MACK
#ifdef _WIN32
    #include "Windows.h"
#else
    #include <limits.h>
    #include <unistd.h>
#endif

namespace sim {

QString Directory::m_binPath = "";

QString Directory::getSrcMazeAlgosDirectory() {
    return getProjectDirectory() + "src/maze/algos/";
}

QString Directory::getResDirectory() {
    return getProjectDirectory() + "res/";
}

QString Directory::getResFontsDirectory() {
    return getResDirectory() + "fonts/";
}

QString Directory::getResMazeDirectory() {
    return getResDirectory() + "maze/";
}

QString Directory::getResMouseDirectory() {
    return getResDirectory() + "mouse/";
}

QString Directory::getResShadersDirectory() {
// XXX: Temporary workaround for OSX
#ifdef __APPLE__
    return getResDirectory() + "shaders/OSX/";
#else
    return getResDirectory() + "shaders/";
#endif
}

QString Directory::getResImgsDirectory() {
    return getResDirectory() + "imgs/";
}

QString Directory::getRunDirectory() {
    return getProjectDirectory() + "run/";
}

void Directory::setBinPath(const QString& path) {
    m_binPath = path;
}

QString Directory::getProjectDirectory() {
    return m_binPath.left(m_binPath.lastIndexOf("/") + 1) + QString("../");
}

} // namespace sim
