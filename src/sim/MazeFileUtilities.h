#pragma once

#include <QByteArray>
#include <QString>
#include <QVector>

#include "BasicTile.h"
#include "MazeFileType.h"

namespace sim {

class MazeFileUtilities {

public:

    MazeFileUtilities() = delete;

    // TODO: MACK - clean this API up
    static QVector<QVector<BasicTile>> loadFromFile(
        const QString& mazeFilePath);

    static QVector<QVector<BasicTile>> loadFromBytes(
        const QByteArray& bytes);

    static bool saveMaze( // TODO: MACK
        const QVector<QVector<BasicTile>>& maze,
        const QString& mazeFilePath,
        MazeFileType mazeFileType);

private:

    // TODO: MACK - define a struct for a basic maze
    static QVector<QVector<BasicTile>> deserializeMapType(
        const QByteArray& bytes);
    static QVector<QVector<BasicTile>> deserializeMazType(
        const QByteArray& bytes);
    static QVector<QVector<BasicTile>> deserializeMz2Type(
        const QByteArray& bytes);
    static QVector<QVector<BasicTile>> deserializeNumType(
        const QByteArray& bytes);

    static QByteArray serializeMapType(
        const QVector<QVector<BasicTile>>& maze);
    static QByteArray serializeMazType(
        const QVector<QVector<BasicTile>>& maze);
    static QByteArray serializeMz2Type(
        const QVector<QVector<BasicTile>>& maze);
    static QByteArray serializeNumType(
        const QVector<QVector<BasicTile>>& maze);
};

} // namespace sim
