#pragma once

#include <QObject>
#include <QPair>
#include <QThread>
#include <QVector>

#include "MLC.h"
#include "MainWindow.h"
#include "Maze.h"

namespace mms {

class ControllerManager : public QObject {

    Q_OBJECT

public:
    ControllerManager(const Maze* maze, MainWindow* window);

    void spawnMouseAlgo(const QString& mouseAlgorithm);

private:
    const Maze* m_maze;
    MainWindow* m_window;

    // A list of MLCs and the threads they're running on
    QVector<QPair<MLC, QThread*>> m_controllers;

};

} // namespace mms
