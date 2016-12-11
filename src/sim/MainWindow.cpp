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

MainWindow::MainWindow(Model* model, Lens* lens, QWidget *parent) :
        QMainWindow(parent),
        m_model(model),
        m_lens(lens),
        ui(new Ui::MainWindow) {

    // Initialization
    ui->setupUi(this);

    // Add header info to the UI
    QVector<QPair<QString, QVariant>> labels = getHeaderInfo();
    int itemsPerColumn = qCeil(labels.size() / 4.0);
    for (int i = 0; i < labels.size(); i += 1) {
        int row = i; //  % itemsPerColumn; // TODO: MACK
        int col = 0; // 2 * (i / itemsPerColumn); // TODO: MACK
        QString label = labels.at(i).first;
        QLabel* labelHolder = new QLabel(label + (label.isEmpty() ? "" : ":"));
        QLabel* valueHolder = new QLabel();
        ui->infoContainer->addWidget(labelHolder, row, col);
        ui->infoContainer->addWidget(valueHolder, row, col + 1);
        m_headerItems.insert(label, valueHolder);
    }

    // Periodically update the header
    connect(
        &m_headerRefreshTimer,
        &QTimer::timeout,
        this,
        [=](){
            QVector<QPair<QString, QVariant>> items = getHeaderInfo();
            for (const auto& pair : items) {
                QString text = pair.second.toString();
                if (pair.second.type() == QVariant::Double) {
                    text = QString::number(pair.second.toDouble(), 'f', 3);
                }
                m_headerItems.value(pair.first)->setText(text);
            }
        }
    );
    m_headerRefreshTimer.start(33);

    // Add a map to the UI
    Map* map = new Map(model, lens);
    map->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    map->setMinimumSize(500, 500);
    ui->mapContainer->addWidget(map);
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
    else if (key == Qt::Key_W) {
        // Toggle wireframe mode
        S()->setWireframeMode(!S()->wireframeMode());
        glPolygonMode(GL_FRONT_AND_BACK, S()->wireframeMode() ? GL_LINE : GL_FILL);
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


/*
        QString("Run ID:                      ") + S()->runId(),
        QString("Random Seed:                 ") + QString::number(P()->randomSeed()),

*/

QVector<QPair<QString, QVariant>> MainWindow::getHeaderInfo() const {
    return {

        // Run info
        {"Run ID", S()->runId()},
        {"Random Seed", P()->randomSeed()},
        {"", ""},

        // Maze info
        {
            (P()->useMazeFile() ? "Maze File" : "Maze Algo"),
            (P()->useMazeFile() ? P()->mazeFile() : P()->mazeAlgorithm()),
        },
        {"Maze Width", m_model->getMaze()->getWidth()},
        {"Maze Height", m_model->getMaze()->getHeight()},
        {"Maze Is Official", m_model->getMaze()->isOfficialMaze() ? "TRUE" : "FALSE"},
        {"", ""},

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

        // Mouse progress
        {"Tiles Traversed",
            QString::number(m_model->getWorld()->getNumberOfTilesTraversed()) + " / " +
            QString::number(m_model->getMaze()->getWidth() * m_model->getMaze()->getHeight())
        },
        {"Closest Distance to Center", m_model->getWorld()->getClosestDistanceToCenter()},
        {"Current X (m)", m_model->getMouse()->getCurrentTranslation().getX().getMeters()},
        {"Current Y (m)", m_model->getMouse()->getCurrentTranslation().getY().getMeters()},
        {"Current Rotation (deg)", m_model->getMouse()->getCurrentRotation().getDegreesZeroTo360()},
        {"Current X tile", m_model->getMouse()->getCurrentDiscretizedTranslation().first},
        {"Current Y tile", m_model->getMouse()->getCurrentDiscretizedTranslation().second},
        {"Current Direction",
            DIRECTION_TO_STRING.value(m_model->getMouse()->getCurrentDiscretizedRotation())
        },
        {"Elapsed Real Time", SimUtilities::formatDuration(Time::get()->elapsedRealTime())},
        {"Elapsed Sim Time", SimUtilities::formatDuration(Time::get()->elapsedSimTime())},
        {"Time Since Origin Departure",
            m_model->getWorld()->getTimeSinceOriginDeparture().getSeconds() < 0
            ? "NONE"
            : SimUtilities::formatDuration(m_model->getWorld()->getTimeSinceOriginDeparture())
        },
        {"Best Time to Center",
            m_model->getWorld()->getBestTimeToCenter().getSeconds() < 0
            ? "NONE"
            : SimUtilities::formatDuration(m_model->getWorld()->getBestTimeToCenter())
        },

        // Sim state
        {"Crashed", (S()->crashed() ? "TRUE" : "FALSE")},
        {"Layout Type (l)", LAYOUT_TYPE_TO_STRING.value(S()->layoutType())},
        {"Rotate Zoomed Map (r)", (S()->rotateZoomedMap() ? "TRUE" : "FALSE")},
        {"Zoomed Map Scale (i, o)", QString::number(S()->zoomedMapScale())},
        {"Wall Truth Visible (t)", (S()->wallTruthVisible() ? "TRUE" : "FALSE")},
        {"Tile Colors Visible (c)", (S()->tileColorsVisible() ? "TRUE" : "FALSE")},
        {"Tile Fog Visible (g)", (S()->tileFogVisible() ? "TRUE" : "FALSE")},
        {"Tile Text Visible (x)", (S()->tileTextVisible() ? "TRUE" : "FALSE")},
        {"Tile Distance Visible (d)", (S()->tileDistanceVisible() ? "TRUE" : "FALSE")},
        {"Header Visible (h)", (S()->headerVisible() ? "TRUE" : "FALSE")},
        {"Wireframe Mode (w)", (S()->wireframeMode() ? "TRUE" : "FALSE")},
        {"Paused (p)", (S()->paused() ? "TRUE" : "FALSE")},
        {"Sim Speed (f, s)", QString::number(S()->simSpeed())},
    };
}

} // namespace mms
