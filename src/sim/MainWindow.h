#pragma once

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPair>
#include <QThread>
#include <QTimer>
#include <QVector>

#include "Controller.h"
#include "Map.h"
#include "Maze.h"
#include "MazeView.h"
#include "MazeViewMutable.h"
#include "MouseGraphic.h"

namespace Ui {
class MainWindow;
}

namespace mms {

class MainWindow : public QMainWindow {

    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

    // Method that gets called on all events (allows us to
    // effectively capture key-press and key-release events)
    bool eventFilter(QObject *object, QEvent *e);

private:

    // The UI object
    Ui::MainWindow *ui;

    // The map object
    Map* m_map;

    // The maze
    Maze* m_maze;

    // The true view of the maze
    MazeView* m_truth;

    // The mouse, its graphic, its view of the maze, and the controller
    // responsible for spawning and interfacing with the mouse algorithm
    Mouse* m_mouse;
    MouseGraphic* m_mouseGraphic;
    MazeViewMutable* m_view;
    Controller* m_controller;

    // Key related helpers
    void keyPress(int key);
    void keyRelease(int key);

    // Stops the currently executing mouse algorithm
    void killMouseAlgorithm();

    // ------- TODO

    void refreshMazeFiles();

    // ------- TODO

    // Maze algo building
    void mazeBuild(const QString& mazeAlgoName);
    QProcess* m_mazeBuildProcess;

    // Maze algo running
    void mazeRun(const QString& mazeAlgoName);
    QProcess* m_mazeRunProcess;

    // ------- TODO

    // Mouse algo building
    void build(const QString& algoName);
    QProcess* m_buildProcess;

    // Mouse algo running
    void spawnMouseAlgo(const QString& algoName);
    QThread* m_mouseAlgoThread;

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

};

} // namespace mms
