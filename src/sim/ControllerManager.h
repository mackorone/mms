#pragma once

#include <QObject>
#include <QThread>
#include <QVector>

#include "Controller.h"
#include "Lens.h"
#include "Maze.h"
#include "Mouse.h"

namespace mms {

class ControllerManager : public QObject {

    Q_OBJECT

public:

    ControllerManager(
        const Maze* maze,
        Mouse* mouse,
        Lens* lens);

    Controller* spawnMouseAlgo(const QString& mouseAlgorithm);

private:

    // The pointers we pass to each controller
    const Maze* m_maze;
    Mouse* m_mouse;
    Lens* m_lens;

    // A list of controllers and the threads they're running on
    QVector<QPair<Controller*, QThread*>> m_controllers;

};

} // namespace mms
