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

    // TODO: MACK - then name of this
    void newMLC(MLC mlc);

    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    const Maze* m_maze;
    Ui::MainWindow *ui;

    // TODO: MACK
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
