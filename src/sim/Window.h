#pragma once

#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QRadioButton>
#include <QThread>

#include "ConfigDialogField.h"
#include "Map.h"
#include "Maze.h"
#include "MazeView.h"
#include "Model.h"
#include "MouseAlgoStatsWidget.h"
#include "MouseGraphic.h"
#include "MouseInterface.h"
#include "RandomSeedWidget.h"

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

    // The game model
    Model m_model;

    // Maze stats GUI elements
    QLabel* m_mazeWidthLabel;
    QLabel* m_mazeHeightLabel;
    QLabel* m_maxDistanceLabel;
    QLabel* m_mazeDirLabel;
    QLabel* m_isValidLabel;
    QLabel* m_isOfficialLabel;

    // The map object
    Map m_map;

    // Some map GUI elements
    QRadioButton* m_truthButton;
    QRadioButton* m_viewButton;
    QCheckBox* m_distancesCheckbox;
    QCheckBox* m_wallTruthCheckbox;
    QCheckBox* m_colorCheckbox;
    QCheckBox* m_fogCheckbox;
    QCheckBox* m_textCheckbox;
    QCheckBox* m_followCheckbox;

    // The maze and the true view of the maze
    Maze* m_maze;
    MazeView* m_truth;

    // The mouse, its graphic, its view of the maze, and the controller
    // responsible for spawning and interfacing with the mouse algorithm
    Mouse* m_mouse;
    MouseGraphic* m_mouseGraphic;
    MazeView* m_view;
    MouseInterface* m_mouseInterface;

    // Helper function for updating the maze 
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
    void cancelBuild();
    void onBuildExit(int exitCode, QProcess::ExitStatus exitStatus);

    // Mouse algo running
    QStringList m_stderrBuffer;
    QProcess* m_mouseAlgoRunProcess;
    QPushButton* m_mouseAlgoRunButton;
    QLabel* m_mouseAlgoRunStatus;
    QPlainTextEdit* m_mouseAlgoRunOutput;
    MouseAlgoStatsWidget* m_mouseAlgoStatsWidget;
    void mouseAlgoRunStart();
    void mouseAlgoRunStop();
    void handleMouseAlgoCannotStart(QString errorString);

    void mouseAlgoPause();
    void mouseAlgoResume();
    QPushButton* m_mouseAlgoPauseButton;
    RandomSeedWidget* m_mouseAlgoSeedWidget;
    QVector<QPushButton*> m_mouseAlgoInputButtons;

    void mouseAlgoRefresh(const QString& name = "");
    QVector<ConfigDialogField> mouseAlgoGetFields();

    // Given some text (and a buffer containing past input), return
    // all complete lines and append remaining text to the buffer
    QStringList getLines(const QString& text, QStringList* buffer);

    // ----- Misc ----- //

    QMap<QString, QLabel*> m_runStats;
    QPair<QStringList, QVector<QVariant>> getRunStats() const;
};

} // namespace mms
