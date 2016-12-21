#pragma once

#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QTimer>

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

    // Inform the UI about a new mouse/lens/controller trio
    void newMLC(MLC mlc);

protected:

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:

    // The UI object
    Ui::MainWindow *ui;

    // The maze
    const Maze* m_maze;

    // The mouse/lens/controller trio
    MLC m_mlc;

    // Header-related members
    // TODO: MACK - refactor this into its own class

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
