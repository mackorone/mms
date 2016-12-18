#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QtCore>

#include "Map.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "Time.h"

namespace mms {

MainWindow::MainWindow(
        const Model* model,
        const Maze* maze,
        Mouse* mouse,
        Lens* lens,
        Controller* controller,
        QWidget *parent) :
        QMainWindow(parent),
        m_model(model),
        m_maze(maze),
        m_mouse(mouse),
        m_lens(lens),
        ui(new Ui::MainWindow) {

    // Initialization
    ui->setupUi(this);

    // TODO: MACK
    connect(
        controller,
        &Controller::algoStdout,
        ui->stdoutTextEdit,
        &QPlainTextEdit::appendPlainText
    );

    // TODO: MACK - clean this up

    // Add run stats info to the UI
    QVector<QPair<QString, QVariant>> runStats = getRunStats();
    for (int i = 0; i < runStats.size(); i += 1) {
        QString label = runStats.at(i).first;
        QLabel* labelHolder = new QLabel(label + ":");
        QLabel* valueHolder = new QLabel();
        ui->runStatsLayout->addWidget(labelHolder, i, 0);
        ui->runStatsLayout->addWidget(valueHolder, i, 1);
        m_runStats.insert(label, valueHolder);
    }

    // Add run stats info to the UI
    QVector<QPair<QString, QVariant>> mazeInfo = getMazeInfo();
    for (int i = 0; i < mazeInfo.size(); i += 1) {
        QString label = mazeInfo.at(i).first;
        QLabel* labelHolder = new QLabel(label + ":");
        QLabel* valueHolder = new QLabel();
        ui->mazeInfoLayout->addWidget(labelHolder, i, 0);
        ui->mazeInfoLayout->addWidget(valueHolder, i, 1);
        m_mazeInfo.insert(label, valueHolder);
    }

    // Periodically update the header
    connect(
        &m_headerRefreshTimer,
        &QTimer::timeout,
        this,
        [=](){
            // TODO: MACK - only update if tab is visible
            QVector<QPair<QString, QVariant>> runStats = getRunStats();
            for (const auto& pair : runStats) {
                QString text = pair.second.toString();
                if (pair.second.type() == QVariant::Double) {
                    text = QString::number(pair.second.toDouble(), 'f', 3);
                }
                m_runStats.value(pair.first)->setText(text);
            }
            QVector<QPair<QString, QVariant>> mazeInfo = getMazeInfo();
            for (const auto& pair : mazeInfo) {
                QString text = pair.second.toString();
                if (pair.second.type() == QVariant::Double) {
                    text = QString::number(pair.second.toDouble(), 'f', 3);
                }
                m_mazeInfo.value(pair.first)->setText(text);
            }
        }
    );
    m_headerRefreshTimer.start(33);

    // Add a map to the UI
    Map* map = new Map(maze, mouse, lens);
    map->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    map->setMinimumSize(300, 300);
    ui->mapContainer->addWidget(map);

    // TODO: MACK - lastly, resize the window
    resize(1200, 633);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

    // NOTE: If you're adding or removing anything from this function, make
    // sure to update wiki/Keys.md

    int key = event->key();

    if (key == Qt::Key_P) {
        // Toggle pause (only in discrete mode)
        // TODO: MACK
        /*
        if (
            STRING_TO_INTERFACE_TYPE.value(m_controllerManager->getStaticOptions().interfaceType)
            == InterfaceType::DISCRETE
        ) {
        */
            S()->setPaused(!S()->paused());
        /*
        }
        else {
            qWarning().noquote().nospace()
                << "Pausing the simulator is only allowed in "
                << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
                << " mode.";
        }
        */
    }
    else if (key == Qt::Key_F) {
        // Faster (only in discrete mode)
        // TODO: MACK
        /*
        if (
            STRING_TO_INTERFACE_TYPE.value(m_controllerManager->getStaticOptions().interfaceType)
            == InterfaceType::DISCRETE
        ) {
        */
            S()->setSimSpeed(S()->simSpeed() * 1.5);
        /*
        }
        else {
            qWarning().noquote().nospace()
                << "Increasing the simulator speed is only allowed in "
                << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
                << " mode.";
        }
        */
    }
    else if (key == Qt::Key_S) {
        // Slower (only in discrete mode)
        // TODO: MACK
        /*
        if (
            STRING_TO_INTERFACE_TYPE.value(m_controllerManager->getStaticOptions().interfaceType)
            == InterfaceType::DISCRETE
        ) {
        */
            S()->setSimSpeed(S()->simSpeed() / 1.5);
        /*
        }
        else {
            qWarning().noquote().nospace()
                << "Decreasing the simulator speed is only allowed in "
                << INTERFACE_TYPE_TO_STRING.value(InterfaceType::DISCRETE)
                << " mode.";
        }
        */
    }
    else if (key == Qt::Key_L) {
        // Cycle through the available layouts
        S()->setLayoutType(LAYOUT_TYPE_CYCLE.value(S()->layoutType()));
    }
    else if (key == Qt::Key_R) {
        // Toggle rotate zoomed map
        S()->setRotateZoomedMap(!S()->rotateZoomedMap());
    }
    else if (key == Qt::Key_I) {
        // Zoom in
        S()->setZoomedMapScale(S()->zoomedMapScale() * 1.5);
    }
    else if (key == Qt::Key_O) {
        // Zoom out
        S()->setZoomedMapScale(S()->zoomedMapScale() / 1.5);
    }
    else if (key == Qt::Key_T) {
        // Toggle wall truth visibility
        S()->setWallTruthVisible(!S()->wallTruthVisible());
        m_lens->getMazeGraphic()->updateWalls();
    }
    else if (key == Qt::Key_C) {
        // Toggle tile colors
        S()->setTileColorsVisible(!S()->tileColorsVisible());
        m_lens->getMazeGraphic()->updateColor();
    }
    else if (key == Qt::Key_G) {
        // Toggle tile fog
        S()->setTileFogVisible(!S()->tileFogVisible());
        m_lens->getMazeGraphic()->updateFog();
    }
    else if (key == Qt::Key_X) {
        // Toggle tile text
        S()->setTileTextVisible(!S()->tileTextVisible());
        m_lens->getMazeGraphic()->updateText();
    }
    else if (key == Qt::Key_D) {
        // Toggle tile distance visibility
        S()->setTileDistanceVisible(!S()->tileDistanceVisible());
        m_lens->getMazeGraphic()->updateText();
    }
    else if (key == Qt::Key_H) {
        // Toggle header visibility
        // TODO: MACK
        /*
        S()->setHeaderVisible(!S()->headerVisible());
        m_header->updateLinesAndColumnStartingPositions();
        */
    }
    else if (key == Qt::Key_Q) {
        // Quit
        SimUtilities::quit();
    }
    else if (
        key == Qt::Key_0 || key == Qt::Key_1 ||
        key == Qt::Key_2 || key == Qt::Key_3 ||
        key == Qt::Key_4 || key == Qt::Key_5 ||
        key == Qt::Key_6 || key == Qt::Key_7 ||
        key == Qt::Key_8 || key == Qt::Key_9
    ) {
        // Press an input button
        int inputButton = key - Qt::Key_0;
        if (!S()->inputButtonWasPressed(inputButton)) {
            S()->setInputButtonWasPressed(inputButton, true);
            qInfo().noquote().nospace()
                << "Input button " << inputButton << " was pressed.";
        }
        else {
            qWarning().noquote().nospace()
                << "Input button " << inputButton << " has not yet been"
                << " acknowledged as pressed; pressing it has no effect.";
        }
    }
}

// TODO: MACK
/*
void Map::specialKeyPress(int key, int x, int y) {
    if (!INT_TO_KEY.contains(key)) {
        return;
    }
    if (ARROW_KEYS.contains(INT_TO_KEY.value(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.value(key), true);
    }
}

void Map::specialKeyRelease(int key, int x, int y) {
    if (!INT_TO_KEY.contains(key)) {
        return;
    }
    if (ARROW_KEYS.contains(INT_TO_KEY.value(key))) {
        S()->setArrowKeyIsPressed(INT_TO_KEY.value(key), false);
    }
}
*/


QVector<QPair<QString, QVariant>> MainWindow::getRunStats() const {
    // TODO: MACK
    MouseStats stats = m_model->getMouseStats("");

    return {
        {"Run ID", S()->runId()}, // TODO: MACK - run directory
        {"Random Seed", P()->randomSeed()},
        {"Mouse Algo", P()->mouseAlgorithm()},
        {"Tiles Traversed",
            QString::number(stats.traversedTileLocations.size()) + " / " +
            QString::number(m_maze->getWidth() * m_maze->getHeight())
        },
        {"Closest Distance to Center", stats.closestDistanceToCenter},
        {"Current X (m)", m_mouse->getCurrentTranslation().getX().getMeters()},
        {"Current Y (m)", m_mouse->getCurrentTranslation().getY().getMeters()},
        {"Current Rotation (deg)", m_mouse->getCurrentRotation().getDegreesZeroTo360()},
        {"Current X tile", m_mouse->getCurrentDiscretizedTranslation().first},
        {"Current Y tile", m_mouse->getCurrentDiscretizedTranslation().second},
        {"Current Direction",
            DIRECTION_TO_STRING.value(m_mouse->getCurrentDiscretizedRotation())
        },
        {"Elapsed Real Time", SimUtilities::formatDuration(Time::get()->elapsedRealTime())},
        {"Elapsed Sim Time", SimUtilities::formatDuration(Time::get()->elapsedSimTime())},
        {"Time Since Origin Departure",
            stats.timeOfOriginDeparture.getSeconds() < 0
            ? "NONE"
            : SimUtilities::formatDuration(
                Time::get()->elapsedSimTime() - stats.timeOfOriginDeparture)
        },
        {"Best Time to Center",
            stats.bestTimeToCenter.getSeconds() < 0
            ? "NONE"
            : SimUtilities::formatDuration(stats.bestTimeToCenter)
        },
        {"Crashed", (S()->crashed() ? "TRUE" : "FALSE")},
    };
}

QVector<QPair<QString, QVariant>> MainWindow::getAlgoOptions() const {
    return {
        // Mouse Info
        {"Mouse Algo", P()->mouseAlgorithm()},
        {"", ""},
        /*
        {"Mouse File", (
            m_controllerManager == nullptr
            ? "NONE"
            : m_controllerManager->getStaticOptions().mouseFile
        }),
        */
        /*
        QString("Interface Type:              ") + (m_controllerManager == nullptr ? "NONE" :
            m_controllerManager->getStaticOptions().interfaceType),
        QString("Initial Direction:           ") + (m_controllerManager == nullptr ? "NONE" :
            m_controllerManager->getStaticOptions().initialDirection),
        QString("Tile Text Num Rows:          ") + (m_controllerManager == nullptr ? "NONE" :
            QString::number(m_controllerManager->getStaticOptions().tileTextNumberOfRows)),
        QString("Tile Text Num Cols:          ") + (m_controllerManager == nullptr ? "NONE" :
            QString::number(m_controllerManager->getStaticOptions().tileTextNumberOfCols)),
        QString("Allow Omniscience:           ") + (m_controllerManager == nullptr ? "NONE" :
            (m_controllerManager->getDynamicOptions().allowOmniscience ? "TRUE" : "FALSE")),
        QString("Auto Clear Fog:              ") + (m_controllerManager == nullptr ? "NONE" :
            (m_controllerManager->getDynamicOptions().automaticallyClearFog ? "TRUE" : "FALSE")),
        QString("Declare Both Wall Halves:    ") + (m_controllerManager == nullptr ? "NONE" :
            (m_controllerManager->getDynamicOptions().declareBothWallHalves ? "TRUE" : "FALSE")),
        QString("Auto Set Tile Text:          ") + (m_controllerManager == nullptr ? "NONE" :
            (m_controllerManager->getDynamicOptions().setTileTextWhenDistanceDeclared ? "TRUE" : "FALSE")),
        QString("Auto Set Tile Base Color:    ") + (m_controllerManager == nullptr ? "NONE" :
            (m_controllerManager->getDynamicOptions().setTileBaseColorWhenDistanceDeclaredCorrectly ? "TRUE" : "FALSE")),
        QString("Wheel Speed Fraction:        ") +
            (m_controllerManager == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE.value(m_controllerManager->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            QString::number(m_controllerManager->getStaticOptions().wheelSpeedFraction))),
        QString("Declare Wall On Read:        ") +
            (m_controllerManager == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE.value(m_controllerManager->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            (m_controllerManager->getDynamicOptions().declareWallOnRead ? "TRUE" : "FALSE"))),
        QString("Use Tile Edge Movements:     ") +
            (m_controllerManager == nullptr ? "NONE" :
            (STRING_TO_INTERFACE_TYPE.value(m_controllerManager->getStaticOptions().interfaceType) != InterfaceType::DISCRETE ? "N/A" :
            (m_controllerManager->getDynamicOptions().useTileEdgeMovements ? "TRUE" : "FALSE"))),
        */
    };
}

QVector<QPair<QString, QVariant>> MainWindow::getMazeInfo() const {
    return {
        {
            (P()->useMazeFile() ? "Maze File" : "Maze Algo"),
            (P()->useMazeFile() ? P()->mazeFile() : P()->mazeAlgorithm()),
        },
        {"Maze Width", m_maze->getWidth()},
        {"Maze Height", m_maze->getHeight()},
        {"Maze Is Official", m_maze->isOfficialMaze() ? "TRUE" : "FALSE"},
    };
}

QVector<QPair<QString, QVariant>> MainWindow::getOptions() const {
    return {
        // Sim state
        {"Layout Type (l)", LAYOUT_TYPE_TO_STRING.value(S()->layoutType())},
        {"Rotate Zoomed Map (r)", (S()->rotateZoomedMap() ? "TRUE" : "FALSE")},
        {"Zoomed Map Scale (i, o)", QString::number(S()->zoomedMapScale())},
        {"Wall Truth Visible (t)", (S()->wallTruthVisible() ? "TRUE" : "FALSE")},
        {"Tile Colors Visible (c)", (S()->tileColorsVisible() ? "TRUE" : "FALSE")},
        {"Tile Fog Visible (g)", (S()->tileFogVisible() ? "TRUE" : "FALSE")},
        {"Tile Text Visible (x)", (S()->tileTextVisible() ? "TRUE" : "FALSE")},
        {"Tile Distance Visible (d)", (S()->tileDistanceVisible() ? "TRUE" : "FALSE")},
        {"Header Visible (h)", (S()->headerVisible() ? "TRUE" : "FALSE")}, // TODO: MACK
        {"Paused (p)", (S()->paused() ? "TRUE" : "FALSE")},
        {"Sim Speed (f, s)", QString::number(S()->simSpeed())},
    };
}

} // namespace mms
