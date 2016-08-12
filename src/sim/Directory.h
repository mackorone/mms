#pragma once

#include <QString>

namespace mms {

class Directory {

public:

    // Initialize the Directory singleton by passing in the
    // absolute path to the project applicationFilePath directory
    static void init(const QString& applicationFilePath);

    // Retrieve the Directory singleton
    static Directory* get();

    // mms/maze/algos
    QString getSrcMazeAlgosDirectory();

    // mms/res
    QString getResDirectory();

    // mms/res/fonts
    QString getResFontsDirectory();

    // mms/res/imgs
    QString getResImgsDirectory();

    // mms/res/maze
    QString getResMazeDirectory();

    // mms/res/mouse
    QString getResMouseDirectory();

    // mms/res/shaders
    QString getResShadersDirectory();

    // mms/run
    QString getRunDirectory();

private:

    // A private constructor is used to ensure
    // only one instance of this class exists
    Directory(const QString& applicationFilePath);

    // A pointer to the actual instance of the class
    static Directory* INSTANCE;

    // mms
    QString m_root;

};

} // namespace mms
