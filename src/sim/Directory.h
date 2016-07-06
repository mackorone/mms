#pragma once

#include <QString>

namespace sim {

class Directory {

public:

    // The Directory class is not constructible
    Directory() = delete;

    // mms/maze/algos
    static QString getSrcMazeAlgosDirectory();

    // mms/res
    static QString getResDirectory();

    // mms/res/fonts
    static QString getResFontsDirectory();

    // mms/res/maze
    static QString getResMazeDirectory();

    // mms/res/mouse
    static QString getResMouseDirectory();

    // mms/res/shaders
    static QString getResShadersDirectory();

    // mms/res/imgs
    static QString getResImgsDirectory();

    // mms/run
    static QString getRunDirectory();

    // TODO: MACK
    static void setBinPath(const QString& path);

private:
    static QString m_binPath; // TODO: MACK

    // mms
    static QString getProjectDirectory();

};

} // namespace sim
