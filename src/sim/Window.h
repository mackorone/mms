#pragma once

#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QQueue>
#include <QRadioButton>
#include <QTimer>

#include "Assert.h"
#include "ConfigDialogField.h"
#include "Map.h"
#include "Maze.h"
#include "MazeView.h"
#include "MouseGraphic.h"
#include "MouseInterface.h"

namespace mms {

class Window : public QMainWindow {

    Q_OBJECT

public:

    Window(QWidget* parent = 0);
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);

private:

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

    // Helper function for updating the maze 
    void loadMazeFile(QString path);
    void setMaze(Maze* maze);

    // ----- MouseAlgosTab ----- //

    QWidget* m_mouseAlgoWidget;
    QComboBox* m_mouseAlgoComboBox;
    QPushButton* m_mouseAlgoEditButton;
    QPushButton* m_mouseAlgoImportButton;
    QTabWidget* m_mouseAlgoOutputTabWidget;
    void mouseAlgoTabInit();
    void mouseAlgoEdit();
    void mouseAlgoImport();

    // Build-related members
    QProcess* m_buildProcess;
    QPushButton* m_buildButton;
    QLabel* m_buildStatus;
    QPlainTextEdit* m_buildOutput;
    void startBuild();
    void onBuildExit(int exitCode, QProcess::ExitStatus exitStatus);

    // Run-related members
    MouseInterface* m_runInterface;
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
    double m_fracToMove;
    QDoubleSpinBox* m_speedBox;
    void scheduleAnotherCall();

    // TODO: MACK

    void startRun();
    void onRunExit(int exitCode, QProcess::ExitStatus exitStatus);
    void handleMouseAlgoCannotStart(QString errorString);

    void removeMouseFromMaze();

    // Cancel running processes
    void cancelAllProcesses();
    void cancelBuild();
    void cancelRun();
    void cancelProcess(QProcess* process, QLabel* status);

    void mouseAlgoPause();
    void mouseAlgoResume();
    QPushButton* m_mouseAlgoPauseButton;

    void mouseAlgoRefresh(const QString& name = "");
    QVector<ConfigDialogField> mouseAlgoGetFields();

    QStringList processText(const QString& text);
};

} 
