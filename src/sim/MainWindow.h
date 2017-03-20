#pragma once

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPair>
#include <QThread>
#include <QTimer>
#include <QVector>

#include "ConfigDialog.h"
#include "Controller.h"
#include "Map.h"
#include "Maze.h"
#include "MazeView.h"
#include "MazeViewMutable.h"
#include "MouseGraphic.h"
#include "ui_mainwindow.h"

namespace mms {

class MainWindow : public QMainWindow {

    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    // The UI object
    Ui::MainWindow ui;

    // The map object
    Map m_map;

    // The maze and the true view of the maze
    Maze* m_maze;
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

    // TODO: MACK - hastily implemented helper functions
    void setMaze(Maze* maze);
    void refreshSettingsMazeFiles();
    void togglePause();

    // Maze algo
    void importMazeAlgo();
    void editMazeAlgo();
    void buildMazeAlgo();
    void runMazeAlgo();
    QProcess* m_buildMazeAlgoProcess;
    QProcess* m_runMazeAlgoProcess;

    // Mouse algo
    void importMouseAlgo();
    void editMouseAlgo();
    void buildMouseAlgo();
    void runMouseAlgo();
    void stopMouseAlgo();
    QProcess* m_buildMouseAlgoProcess;
    QThread* m_runMouseAlgoThread;

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
