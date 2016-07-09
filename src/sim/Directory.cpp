#include "Directory.h"

#include "Assert.h"

namespace sim {

Directory* Directory::INSTANCE = nullptr; 

void Directory::init(const QString& root) {
    SIM_ASSERT_TR(INSTANCE == nullptr);
    INSTANCE = new Directory(root);
}

Directory* Directory::get() {
    SIM_ASSERT_FA(INSTANCE == nullptr);
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

QString Directory::getResMazeDirectory() {
    return getResDirectory() + "maze/";
}

QString Directory::getResMouseDirectory() {
    return getResDirectory() + "mouse/";
}

QString Directory::getResShadersDirectory() {
    return getResDirectory() + "shaders/";
}

QString Directory::getRunDirectory() {
    return m_root + "run/";
}

Directory::Directory(const QString& root) :
    m_root(root) {
}

} // namespace sim
