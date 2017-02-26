#include "Directory.h"

#include "Assert.h"

namespace mms {

Directory* Directory::INSTANCE = nullptr; 

void Directory::init(const QString& applicationFilePath) {
    ASSERT_TR(INSTANCE == nullptr);
    INSTANCE = new Directory(applicationFilePath);
}

Directory* Directory::get() {
    ASSERT_FA(INSTANCE == nullptr);
    return INSTANCE;
}

QString Directory::getSrcMazeAlgosDirectory() {
    return m_root + "src/maze/algos/";
}

QString Directory::getResDirectory() {
    return m_root + "res/";
}

QString Directory::getResFontsDirectory() {
    return getResDirectory() + "fonts/";
}

QString Directory::getResImgsDirectory() {
    return getResDirectory() + "imgs/";
}

QString Directory::getResMouseDirectory() {
    return getResDirectory() + "mouse/";
}

QString Directory::getRunDirectory() {
    return m_root + "run/";
}

Directory::Directory(const QString& applicationFilePath) {
    QString path = applicationFilePath;
    path = path.left(path.lastIndexOf("/")); // Strips off /bin
    path = path.left(path.lastIndexOf("/")); // Strips off /sim
    m_root = path + "/";
}

} // namespace mms
