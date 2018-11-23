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
#include "MouseAlgoStatsWidget.h"
#include "MouseGraphic.h"
#include "MouseInterface.h"

namespace mms {

class Window : public QMainWindow {

    Q_OBJECT

public:

    Window(QWidget* parent = 0);
    void closeEvent(QCloseEvent* event);
    void resizeEvent(QResizeEvent* event);

signals:

    // Emits this signal when a user presses an input button
    void inputButtonWasPressed(int button);

    // Emits this signal when the mouse algo can't start
    void mouseAlgoCannotStart(QString errorString);

private:

    // The map object
    Map m_map;

    // Some map GUI elements
    QRadioButton* m_truthButton;
    QRadioButton* m_viewButton;

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

    // Helper function for editing settings
    void editSettings();

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
    void cancelBuild();
    void cancelRun();
    void cancelProcess(QProcess* process, QLabel* status);

    MouseAlgoStatsWidget* m_mouseAlgoStatsWidget;

    void mouseAlgoPause();
    void mouseAlgoResume();
    QPushButton* m_mouseAlgoPauseButton;
    QVector<QPushButton*> m_mouseAlgoInputButtons;

    void mouseAlgoRefresh(const QString& name = "");
    QVector<ConfigDialogField> mouseAlgoGetFields();

    QStringList processText(const QString& text);

    // ----- Misc ----- //

    QMap<QString, QLabel*> m_runStats;
    QPair<QStringList, QVector<QVariant>> getRunStats() const;
};

} // namespace mms
