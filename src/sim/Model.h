#pragma once

#include <QObject>
#include <QMutex>

#include "Maze.h"
#include "Mouse.h"
#include "MouseStats.h"

namespace mms {

class Model : public QObject {

    Q_OBJECT

public:

    Model();
    void simulate();
    void shutdown();

    void setMaze(const Maze* maze);

    void addMouse(const QString& name, Mouse* mouse);
    void removeMouse(const QString& name);
    bool containsMouse(const QString& name) const;
    MouseStats getMouseStats(const QString& name) const;

signals:

    void newTileLocationTraversed(int x, int y);

private:

    QMutex m_mutex;
    bool m_shutdownRequested;

    const Maze* m_maze;
    QMap<QString, Mouse*> m_mice;
    QMap<QString, MouseStats> m_stats;

    void checkCollision();
};

} // namespace mms
