#pragma once

#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QThread>

#include "Controller.h"
#include "ConfigDialogField.h"
#include "Map.h"
#include "Maze.h"
#include "MazeView.h"
#include "Model.h"
#include "MouseGraphic.h"
#include "RandomSeedWidget.h"
#include "TextDisplayWidget.h"

namespace mms {

class Window : public QMainWindow {

    Q_OBJECT

public:

    Window(QWidget* parent = 0);
    void closeEvent(QCloseEvent* event);

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
    Controller* m_controller;

    // The event loop for the mouse algo process. We need a separate loop so
    // that the GUI doesn't lock up on blocking algo commands, like sleep
    QThread* m_mouseAlgoThread;

    // Helper functions
    void setMaze(Maze* maze);
    void runMouseAlgo(
        const QString& name,
        const QString& runCommand,
        const QString& dirPath,
        const QString& mouseFilePath,
        int seed,
        TextDisplayWidget* display);
    void stopMouseAlgo();

// TODO: MACK ---------------  MazeAlgosTab
private:

    void mazeAlgoTabInit();
    void mazeAlgoEdit();
    void mazeAlgoImport();

    // TODO: MACK
    QWidget* m_mazeAlgoWidget;
    QComboBox* m_mazeAlgoComboBox;
    QPushButton* m_mazeAlgoEditButton;
    QPushButton* m_mazeAlgoImportButton;

	// Functions encapsulating process management logic,
    // shared between maze and mouse algorithms
	void algoActionStart(
		QProcess** actionProcessVariable,
		QPushButton* actionButton,
		QLabel* actionStatus,
		QPlainTextEdit* actionOutput,
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


    /*
    // Key related helpers
    void keyPress(int key);
    void keyRelease(int key);

    // TODO: MACK - hastily implemented helper functions
    void togglePause();

    // Header-related members
    // TODO: MACK - refactor this into its own class

    QGridLayout* m_activeTab; // TODO: MACK
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
