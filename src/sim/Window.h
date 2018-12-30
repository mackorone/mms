#pragma once

#include <QChar>
#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QQueue>
#include <QSet>
#include <QState>
#include <QStateMachine>
#include <QTimer>
#include <QToolButton>

#include "Map.h"
#include "Maze.h"
#include "MazeView.h"
#include "Mouse.h"
#include "MouseGraphic.h"

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

    // ----- Algo config -----

    QComboBox* m_mouseAlgoComboBox;
    QToolButton* m_mouseAlgoEditButton;

    void onMouseAlgoEditButtonPressed();
    void onMouseAlgoImportButtonPressed();
    void refreshMouseAlgoComboBox(QString selected);

    // ----- Algo output -----

    QTabWidget* m_mouseAlgoOutputTabWidget;
    QPlainTextEdit* m_buildOutput;
    QPlainTextEdit* m_runOutput;

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

    // The mouse, its graphic, its view of the maze
    Mouse* m_mouse;
    MouseGraphic* m_mouseGraphic;
    MazeView* m_view;

    // Run-related members
    // TODO: MACK
    void startRun();
    void cancelRun();
    void onRunExit(int exitCode, QProcess::ExitStatus exitStatus);
    void removeMouseFromMaze();

    // ----- Misc -----

    // The OpenGL widget (i.e., the graphics)
    Map* m_map;

    // Cache
    QSet<QPair<int, int>> m_tilesWithColor;
    QSet<QPair<int, int>> m_tilesWithText;

    // Cancel running processes
    void cancelAllProcesses();
    void cancelProcess(QProcess* process, QLabel* status);

    // ----- Communication -----

    QStringList processText(const QString& text);

    // Execute a request, return a response
    QString dispatch(const QString& command);

    // A buffer to hold incomplete commands, ensures we only
    // process commands once they're terminated with a newline
    QStringList m_commandBuffer;

    // Handle a single command
    void processCommand(QString command);

    // For processing serial commands asynchronously
    QQueue<QString> m_commandQueue;
    void processQueuedCommands();
    QTimer* m_commandQueueTimer;

    // ----- Movement -----

    // TODO: MACK
    bool isMoving();
    double progressRemaining();
    void moveALittle(double progress);

    double m_step;
    QSlider* m_speedSlider;
    void scheduleAnotherCall();

    // State used for asynchronous mouse movement
    void updateStartingLocationAndDirection();
    QPair<int, int> m_startingLocation;
    Direction m_startingDirection;
    Movement m_movement;
    double m_progress;
    double progressRequired(Movement movement);

    // ----- Interaction ----

    // TODO: MACK
    void mouseAlgoPause();
    void mouseAlgoResume();
    QPushButton* m_resetButton;
    QPushButton* m_pauseButton;
    bool m_isPaused;
    bool m_wasReset;
    void onPauseButtonPressed();
    void onResetButtonPressed();

    // ----- API -----

    int getWidth();
    int getHeight();

    bool isWallFront();
    bool isWallRight();
    bool isWallLeft();

    bool moveForward();
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

    bool isWall(Wall wall) const;
    bool isWithinMaze(int x, int y) const;
    Wall getOpposingWall(Wall wall) const;
    Coordinate getCenterOfTile(int x, int y) const;
};

} 
