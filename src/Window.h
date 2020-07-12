#pragma once

#include <QChar>
#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QPair>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QQueue>
#include <QSet>
#include <QTimer>
#include <QToolButton>
#include <QGridLayout>

#include "Map.h"
#include "Maze.h"
#include "MazeView.h"
#include "Mouse.h"
#include "MouseGraphic.h"
#include "Stats.h"

namespace mms {

enum class Movement {
    MOVE_FORWARD,
    TURN_RIGHT,
    TURN_LEFT,
    NONE,
};

struct Wall {
    int x;
    int y;
    Direction d;
};

class Window : public QMainWindow {

    Q_OBJECT

public:

    Window(QWidget* parent = 0);
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);

private:

    // ----- Graphics -----

    Map* m_map;
    void scheduleMapUpdate();

    // ----- Maze -----

    Maze* m_maze;
    MazeView* m_truth;
    QString m_currentMazeFile;
    QComboBox* m_mazeFileComboBox;

    void onMazeFileButtonPressed();
    void onMazeFileComboBoxChanged(QString path);
    void showInvalidMazeFileWarning(QString path);
    void refreshMazeFileComboBox(QString selected);
    void updateMazeAndPath(Maze* maze, QString path);
    void updateMaze(Maze* maze);

    // ----- Colors -----

    void onColorButtonPressed();

    // ----- Algo config -----

    QComboBox* m_mouseAlgoComboBox;
    QToolButton* m_mouseAlgoEditButton;

    void onMouseAlgoComboBoxChanged(QString name);
    void onMouseAlgoEditButtonPressed();
    void onMouseAlgoImportButtonPressed();
    void refreshMouseAlgoComboBox(QString selected);

    // ----- Algo processes-----

    static const QString IN_PROGRESS_STYLE_SHEET;
    static const QString COMPLETE_STYLE_SHEET;
    static const QString CANCELED_STYLE_SHEET;
    static const QString FAILED_STYLE_SHEET;
    static const QString ERROR_STYLE_SHEET;

    QTabWidget* m_mouseAlgoOutputTabWidget;
    QPlainTextEdit* m_buildOutput;
    QPlainTextEdit* m_runOutput;

    void cancelProcess(QProcess* process, QLabel* status);
    void cancelAllProcesses();

    // ----- Algo build -----

    QPushButton* m_buildButton;
    QProcess* m_buildProcess;
    QLabel* m_buildStatus;

    void startBuild();
    void cancelBuild();
    void onBuildExit(int exitCode, QProcess::ExitStatus exitStatus);

    // ----- Algo run -----

    QPushButton* m_runButton;
    QProcess* m_runProcess;
    QLabel* m_runStatus;

    void startRun();
    void cancelRun();
    void onRunExit(int exitCode, QProcess::ExitStatus exitStatus);

    Mouse* m_mouse;
    MazeView* m_view;
    MouseGraphic* m_mouseGraphic;

    void removeMouseFromMaze();

    // ----- Pause/reset ----

    bool m_isPaused;
    bool m_wasReset;
    QPushButton* m_pauseButton;
    QPushButton* m_resetButton;

    void onPauseButtonPressed();
    void onResetButtonPressed();

    // ----- Communication -----

    static const QString ACK;
    static const QString CRASH;
    static const QString INVALID;

    // Buffers to hold incomplete output, only
    // process once terminated with a newline
    QStringList m_logBuffer;
    QStringList m_commandBuffer;
    QStringList processText(QString text, QStringList* buffer);

    QQueue<QString> m_commandQueue;
    QTimer* m_commandQueueTimer;

    void dispatchCommand(QString command);
    QString executeCommand(QString command);
    void processQueuedCommands();

    // ----- Movement -----

    static const int SPEED_SLIDER_MAX;
    static const int SPEED_SLIDER_DEFAULT;
    static const double PROGRESS_REQUIRED_FOR_MOVE;
    static const double PROGRESS_REQUIRED_FOR_TURN;
    static const double MIN_PROGRESS_PER_SECOND;
    static const double MAX_PROGRESS_PER_SECOND;
    static const double MAX_SLEEP_SECONDS;

    // TODO: upforgrabs
    // Encapsulate this state in the mouse class

    QPair<int, int> m_startingLocation;
    Direction m_startingDirection;
    Movement m_movement;
    bool m_doomedToCrash; // if the requested movement will result in a crash
    int m_movesRemaining; // the number of allowable forward steps remaining
    double m_movementProgress;
    double m_movementStepSize;
    QSlider* m_speedSlider;

    double progressRequired(Movement movement);
    void updateMouseProgress(double progress);
    void scheduleMouseProgressUpdate();
    bool isMoving();

    // ----- Scoreboard -----
    Stats* stats;
    void createStat(QString name, enum StatsEnum stat, int labelRow, int labelCol, int valueRow, int valueCol, QGridLayout* layout);

    // ----- API -----

    int mazeWidth();
    int mazeHeight();

    bool wallFront(int distance);
    bool wallRight();
    bool wallLeft();

    bool moveForward(int distance);
    void turnRight();
    void turnLeft();

    void setWall(int x, int y, QChar direction);
    void clearWall(int x, int y, QChar direction);

    void setColor(int x, int y, QChar color);
    void clearColor(int x, int y);
    void clearAllColor();

    void setText(int x, int y, QString text);
    void clearText(int x, int y);
    void clearAllText();

    bool wasReset();
    void ackReset();

    // ----- Helpers -----

    QSet<QPair<int, int>> m_tilesWithColor;
    QSet<QPair<int, int>> m_tilesWithText;

    QString boolToString(bool value) const;
    bool isWall(Wall wall) const;
    bool isWithinMaze(int x, int y) const;
    Wall getOpposingWall(Wall wall) const;
    Coordinate getCenterOfTile(int x, int y) const;
};

} 
