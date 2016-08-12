#pragma once

#include <QByteArray>
#include <QString>

#include "BasicMaze.h"
#include "MazeFileType.h"

namespace mms {

class MazeFileUtilities {

public:

    MazeFileUtilities() = delete;

    static BasicMaze load(const QString& path);
    static BasicMaze loadBytes(const QByteArray& bytes);

    static void save(
        const BasicMaze& maze,
        const QString& path,
        MazeFileType type);

private:

    static BasicMaze deserializeMapType(const QByteArray& bytes);
    static BasicMaze deserializeMazType(const QByteArray& bytes);
    static BasicMaze deserializeMz2Type(const QByteArray& bytes);
    static BasicMaze deserializeNumType(const QByteArray& bytes);

    static QByteArray serializeMapType(const BasicMaze& maze);
    static QByteArray serializeMazType(const BasicMaze& maze);
    static QByteArray serializeMz2Type(const BasicMaze& maze);
    static QByteArray serializeNumType(const BasicMaze& maze);
};

} // namespace mms
