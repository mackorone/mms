#pragma once

#include <QCloseEvent>
#include <QMainWindow>
#include <QThread>

#include "Controller.h"
#include "Map.h"
#include "Maze.h"
#include "MazeView.h"
#include "Model.h"
#include "MouseGraphic.h"
#include "TextDisplay.h"

namespace mms {

class Window : public QMainWindow {

    Q_OBJECT

public:

    Window(QWidget* parent = 0);
    void closeEvent(QCloseEvent *event);

private:

    // The model object
    Model m_model;
    QThread m_modelThread;

    // The map object
    Map m_map;

    // The maze and the true view of the maze
    Maze* m_maze;
    MazeView* m_truth;

    // The mouse, its graphic, its view of the maze, and the controller
    // responsible for spawning and interfacing with the mouse algorithm
    Mouse* m_mouse;
    MouseGraphic* m_mouseGraphic;
    MazeView* m_view;
    Controller* m_controller;

    // The event loop for the mouse algo process. We need a separate loop so
    // that the GUI doesn't lock up on blocking algo commands, like sleep
    QThread* m_mouseAlgoThread;

    // Helper functions
    void setMaze(Maze* maze);
    void runMouseAlgo(
        const QString& name,
        const QString& runCommand,
        const QString& dirPath,
        const QString& mouseFilePath,
        int seed,
        TextDisplay* display);
    void stopMouseAlgo();

    /*
    // Key related helpers
    void keyPress(int key);
    void keyRelease(int key);

    // TODO: MACK - hastily implemented helper functions
    void togglePause();

    // Header-related members
    // TODO: MACK - refactor this into its own class

    QGridLayout* m_activeTab; // TODO: MACK
    QMap<QGridLayout*, QMap<QString, QLabel*>> m_stats;

    QMap<QString, QLabel*> m_runStats;
    QMap<QString, QLabel*> m_algoOptions;
    QMap<QString, QLabel*> m_mazeInfo;
    QMap<QString, QLabel*> m_options;

    QVector<QPair<QString, QVariant>> getRunStats() const;
    QVector<QPair<QString, QVariant>> getAlgoOptions() const;
    QVector<QPair<QString, QVariant>> getMazeInfo() const;
    QVector<QPair<QString, QVariant>> getOptions() const;

    QTimer m_headerRefreshTimer;
    */

};

} // namespace mms
