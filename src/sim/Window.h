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

#include "Assert.h"
#include "ConfigDialogField.h"
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

    // ----- UI -----

    // The OpenGL widget (i.e., the graphics)
    Map* m_map;

    // The mouse, its graphic, its view of the maze
    Mouse* m_mouse;
    MouseGraphic* m_mouseGraphic;
    MazeView* m_view;

    // Controls
    QComboBox* m_mouseAlgoComboBox;
    QToolButton* m_mouseAlgoEditButton;
    QToolButton* m_mouseAlgoImportButton;
    QTabWidget* m_mouseAlgoOutputTabWidget;
    void mouseAlgoEdit();
    void mouseAlgoImport();

    // ----- Maze -----

    // The maze and the true view of the maze
    Maze* m_maze;
    MazeView* m_truth;

    // Update the maze
    QString m_currentMazeFile;
    QComboBox* m_mazeFileComboBox;
    void onMazeFileButtonPressed();
    void onMazeFileComboBoxChanged(QString path);
    void showInvalidMazeFileWarning(QString path);
    void refreshMazeFileComboBox(QString selected);
    void updateMazeAndPath(Maze* maze, QString path);
    void updateMaze(Maze* maze);

    // ----- Build -----

    // Build-related members
    QProcess* m_buildProcess;
    QPushButton* m_buildButton;
    QLabel* m_buildStatus;
    QPlainTextEdit* m_buildOutput;
    void startBuild();
    void onBuildExit(int exitCode, QProcess::ExitStatus exitStatus);

    // Run-related members
    QProcess* m_runProcess;
    QPushButton* m_runButton;
    QLabel* m_runStatus;
    QPlainTextEdit* m_runOutput;

    // A buffer to hold incomplete commands, ensures we only
    // process commands once they're terminated with a newline
    QStringList m_commandBuffer;

    // Handle a single command
    void processCommand(QString command);

    // For processing serial commands asynchronously
    QQueue<QString> m_commandQueue;
    void processQueuedCommands();
    QTimer* m_commandQueueTimer;

    // TODO: MACK
    double m_step;
    QSlider* m_speedSlider;
    void scheduleAnotherCall();
    void mouseAlgoPause();
    void mouseAlgoResume();
    QPushButton* m_resetButton;
    QPushButton* m_pauseButton;
    bool m_isPaused;

    // TODO: MACK
    void startRun();
    void onRunExit(int exitCode, QProcess::ExitStatus exitStatus);
    void removeMouseFromMaze();

    // Cancel running processes
    void cancelAllProcesses();
    void cancelBuild();
    void cancelRun();
    void cancelProcess(QProcess* process, QLabel* status);

    void mouseAlgoRefresh(const QString& name = "");
    QVector<ConfigDialogField> mouseAlgoGetFields();

    QStringList processText(const QString& text);

/////////////////////////////////////////////////////////
// TODO: MACK


    // Execute a request, return a response
    QString dispatch(const QString& command);

    // A user pressed an input button in the UI
    void inputButtonWasPressed(int button);

    // TODO: MACK
    bool isMoving();
    double progressRemaining();
    void moveALittle(double progress);

    // State used for asynchronous mouse movement
    void updateStartingLocationAndDirection();
    QPair<int, int> m_startingLocation;
    Direction m_startingDirection;
    Movement m_movement;
    double m_progress;
    double progressRequired(Movement movement);

    // Returns the center of a given tile
    Coordinate getCenterOfTile(int x, int y) const;

    QSet<QPair<int, int>> m_tilesWithColor;
    QSet<QPair<int, int>> m_tilesWithText;

    bool m_wasReset;
    void resetButtonPressed();

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

    // ----- API Helpers -----

    bool isWall(Wall wall) const;
    bool isWithinMaze(int x, int y) const;
    Wall getOpposingWall(Wall wall) const;
};

} 
