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
    void setMouse(Mouse* mouse);
    void removeMouse();

    MouseStats getMouseStats() const;

    void setPaused(bool paused);
    void setSimSpeed(double factor);

signals:

    void newTileLocationTraversed(int x, int y);

private:

    QMutex m_mutex;
    bool m_shutdownRequested;

    const Maze* m_maze;
    Mouse* m_mouse;
    MouseStats* m_stats;

    bool m_paused;
    double m_simSpeed;

    void checkCollision();
};

} // namespace mms
