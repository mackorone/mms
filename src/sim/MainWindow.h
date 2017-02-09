#pragma once

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPair>
#include <QThread>
#include <QTimer>
#include <QVector>

#include "MLC.h"
#include "Maze.h"

namespace Ui {
class MainWindow;
}

namespace mms {

class MainWindow : public QMainWindow {

    Q_OBJECT

public:

    MainWindow(const Maze* maze, QWidget *parent = 0);
    ~MainWindow();

protected:

    // Method that gets called on all events (allows us to
    // effectively capture key-press and key-release events)
    bool eventFilter(QObject *object, QEvent *e);

private:

    // The UI object
    Ui::MainWindow *ui;

    // The maze
    const Maze* m_maze;

    // The mouse/lens/controller trio
    MLC m_mlc;

    // Key related helpers
    void keyPress(int key);
    void keyRelease(int key);

    // Algo building
    void build(const QString& algoName);
    QProcess* m_buildProcess;

    // Algo running
    void spawnMouseAlgo(const QString& algoName);
    QVector<QPair<MLC, QThread*>> m_controllers;

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
