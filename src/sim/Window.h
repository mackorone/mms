#pragma once

#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
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
#include "MouseGraphic.h"
#include "MouseInterface.h"
#include "RandomSeedWidget.h"

namespace mms {

class Window : public QMainWindow {

    Q_OBJECT

public:

    Window(QWidget* parent = 0);
    void closeEvent(QCloseEvent* event);

signals:

    // Emits this signal when a user presses an input button
    void inputButtonWasPressed(int button);

private:

    // The model object
    Model m_model;
    QThread m_modelThread;

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

    // Functions encapsulating process management logic,
    // shared between maze and mouse algorithms
    void algoActionStart(
        QProcess** actionProcessVariable,
        QPushButton* actionButton,
        QLabel* actionStatus,
        QPlainTextEdit* actionOutput,
        QTabWidget* outputTabWidget,
        const QString& algoName,
        const QString& actionName,
        const QString& actionString,
         QString (*getCommand)(const QString&),
         QString (*getDirPath)(const QString&),
        QVector<QString> (Window::*getExtraArgs)(void),
        void (Window::*actionStart)(void),
        void (Window::*actionStop)(void),
        void (Window::*stderrMidAction)(void),
        void (Window::*stderrPostAction)(void)
    );
    void algoActionStop(
        QProcess* actionProcess,
        QLabel* actionStatus
    );

    // ----- MazeAlgosTab ----- //

    QWidget* m_mazeAlgoWidget;
    QComboBox* m_mazeAlgoComboBox;
    QPushButton* m_mazeAlgoEditButton;
    QPushButton* m_mazeAlgoImportButton;
    QTabWidget* m_mazeAlgoOutputTabWidget;
    void mazeAlgoTabInit();
    void mazeAlgoEdit();
    void mazeAlgoImport();

    // Maze algo building
    QProcess* m_mazeAlgoBuildProcess;
    QPushButton* m_mazeAlgoBuildButton;
    QLabel* m_mazeAlgoBuildStatus;
    QPlainTextEdit* m_mazeAlgoBuildOutput;
    void mazeAlgoBuildStart();
    void mazeAlgoBuildStop();
    void mazeAlgoBuildStderr();

    // Maze algo running
    QProcess* m_mazeAlgoRunProcess;
    QPushButton* m_mazeAlgoRunButton;
    QLabel* m_mazeAlgoRunStatus;
    QPlainTextEdit* m_mazeAlgoRunOutput;
    QVector<QString> mazeAlgoRunExtraArgs();
    void mazeAlgoRunStart();
    void mazeAlgoRunStop();
    void mazeAlgoRunStderr();

    QSpinBox* m_mazeAlgoWidthBox;
    QSpinBox* m_mazeAlgoHeightBox;
    RandomSeedWidget* m_mazeAlgoSeedWidget;

    void mazeAlgoRefresh(const QString& name = "");
    QVector<ConfigDialogField> mazeAlgoGetFields();

    // ----- MouseAlgosTab ----- //

    QWidget* m_mouseAlgoWidget;
    QComboBox* m_mouseAlgoComboBox;
    QPushButton* m_mouseAlgoEditButton;
    QPushButton* m_mouseAlgoImportButton;
    QTabWidget* m_mouseAlgoOutputTabWidget;
    void mouseAlgoTabInit();
    void mouseAlgoEdit();
    void mouseAlgoImport();

    // Mouse algo building
    QProcess* m_mouseAlgoBuildProcess;
    QPushButton* m_mouseAlgoBuildButton;
    QLabel* m_mouseAlgoBuildStatus;
    QPlainTextEdit* m_mouseAlgoBuildOutput;
    void mouseAlgoBuildStart();
    void mouseAlgoBuildStop();
    void mouseAlgoBuildStderr();

    // The event loop for the mouse algo process. We need a separate loop so
    // that the GUI doesn't lock up on blocking algo commands, like sleep
    QThread* m_mouseAlgoThread;

    // Mouse algo running
    QStringList m_stderrBuffer;
    QProcess* m_mouseAlgoRunProcess;
    QPushButton* m_mouseAlgoRunButton;
    QLabel* m_mouseAlgoRunStatus;
    QPlainTextEdit* m_mouseAlgoRunOutput;
    void mouseAlgoRunStart();
    void mouseAlgoRunStop();

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

    /*
    // TODO: MACK
    // Key related helpers
    QGridLayout* m_activeTab;
    QMap<QGridLayout*, QMap<QString, QLabel*>> m_stats;
    QMap<QString, QLabel*> m_runStats;
    QMap<QString, QLabel*> m_algoOptions;
    QMap<QString, QLabel*> m_options;
    QVector<QPair<QString, QVariant>> getRunStats() const;
    QVector<QPair<QString, QVariant>> getAlgoOptions() const;
    QTimer m_headerRefreshTimer;
    */
};

} // namespace mms
