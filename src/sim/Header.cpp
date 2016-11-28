#include "Header.h"

#include <QDebug>
#include <QFile>

#include "ControllerManager.h"
#include "Directory.h"
#include "Layout.h"
#include "Logging.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"
#include "Time.h"

namespace mms {

Header::Header(Model* model) :
        m_model(model),
        m_controllerManager(nullptr),
        m_windowWidth(0),
        m_windowHeight(0),
        m_textHeight(P()->headerTextHeight()),
        m_rowSpacing(P()->headerRowSpacing()),
        m_columnSpacing(P()->headerColumnSpacing()) {

    // Check to make sure that the font file exists
    QString fontPath = Directory::get()->getResFontsDirectory() + P()->headerTextFont();
    if (!QFile::exists(fontPath)) {
        // If the font doesn't exist, we simply draw no text whatsoever
        qWarning().noquote().nospace()
            << "\"" << fontPath << "\" is not a valid font file; it's very"
            << " possible that the file does not exist. No header will be"
            << " drawn.";
    }

    // Create the text drawer object
    // TODO: MACK
    // m_textDrawer = new TextDrawer(fontPath, m_textHeight);

    // Populate the lines with initial values
    updateLines();
}

int Header::getHeight() const {
    // TODO: MACK
    return 0;

    int numRows = getNumRows(m_lines.size(), m_columnStartingPositions.size());
    return P()->windowBorderWidth() + numRows * m_textHeight + (numRows - 1) * m_rowSpacing;
}

void Header::setControllerManager(ControllerManager* controllerManager) {
    m_controllerManager = controllerManager;
}

void Header::updateWindowSize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    updateColumnStartingPositions();
}

void Header::updateLinesAndColumnStartingPositions() {
    updateLines();
    updateColumnStartingPositions();
}

void Header::draw() {
    // TODO: MACK
    return;

    // Update the lines of text to be drawn each and every frame
    updateLines();

    // Get the current number of rows (based on current lines and columnStartingPositions)
    int numRows = getNumRows(m_lines.size(), m_columnStartingPositions.size());

    // TODO: MACK
    // Draw all of the text for the frame
    // m_textDrawer->commenceDrawingTextForFrame();
    // for (int i = 0; i < m_columnStartingPositions.size(); i += 1) {
    //     for (int j = 0; j < numRows && i * numRows + j < m_lines.size(); j += 1) {
    //         m_textDrawer->drawText(
    //             m_columnStartingPositions.at(i),
    //             m_windowHeight - P()->windowBorderWidth() - m_textHeight - j * (m_textHeight + m_rowSpacing),
    //             m_windowWidth,
    //             m_windowHeight,
    //             // Prepend a column separator if not the first column
    //             QString((0 < i ? "| " : "")) + m_lines.at(i * numRows + j)
    //         );
    //     }
    // }
    // m_textDrawer->concludeDrawingTextForFrame();
}

int Header::getNumRows(int numLines, int numCols) const {
    return numLines / numCols + (numLines % numCols == 0 ? 0 : 1);
}

void Header::updateLines() {
    // TODO: MACK
    return;

    if (!S()->headerVisible()) {
        m_lines = {};
        return;
    }

    m_lines = {

        // Run info
        QString("Run ID:                      ") + S()->runId(),
        QString("Random Seed:                 ") + QString::number(P()->randomSeed()),

        // Maze info
        (
            P()->useMazeFile() ?
            QString("Maze File:                   ") + P()->mazeFile() :
            QString("Maze Algo:                   ") + P()->mazeAlgorithm()
        ),
        QString("Maze Width:                  ") + QString::number(m_model->getMaze()->getWidth()),
        QString("Maze Height:                 ") + QString::number(m_model->getMaze()->getHeight()),
        QString("Maze Is Official:            ") + (m_model->getMaze()->isOfficialMaze() ? "TRUE" : "FALSE"),

        // Mouse Info
        /*
        QString("Mouse Algo:                  ") + P()->mouseAlgorithm(),

        QString("Mouse File:                  ") + (m_controllerManager == nullptr ? "NONE" :
            m_controllerManager->getStaticOptions().mouseFile),
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
        QString("Tiles Traversed:             ") +
            QString::number(m_model->getWorld()->getNumberOfTilesTraversed()) +
            "/" + QString::number(m_model->getMaze()->getWidth() * m_model->getMaze()->getHeight()),
        QString("Closest Distance to Center:  ") + QString::number(m_model->getWorld()->getClosestDistanceToCenter()),
        QString("Current X (m):               ") + QString::number(m_model->getMouse()->getCurrentTranslation().getX().getMeters()),
        QString("Current Y (m):               ") + QString::number(m_model->getMouse()->getCurrentTranslation().getY().getMeters()),
        QString("Current Rotation (deg):      ") + QString::number(m_model->getMouse()->getCurrentRotation().getDegreesZeroTo360()),
        QString("Current X tile:              ") + QString::number(m_model->getMouse()->getCurrentDiscretizedTranslation().first),
        QString("Current Y tile:              ") + QString::number(m_model->getMouse()->getCurrentDiscretizedTranslation().second),
        QString("Current Direction:           ") + 
            DIRECTION_TO_STRING.value(m_model->getMouse()->getCurrentDiscretizedRotation()),
        QString("Elapsed Real Time:           ") + SimUtilities::formatDuration(Time::get()->elapsedRealTime()),
        QString("Elapsed Sim Time:            ") + SimUtilities::formatDuration(Time::get()->elapsedSimTime()),
        QString("Time Since Origin Departure: ") + (
            m_model->getWorld()->getTimeSinceOriginDeparture().getSeconds() < 0 ? "NONE" :
            SimUtilities::formatDuration(m_model->getWorld()->getTimeSinceOriginDeparture())
        ),
        QString("Best Time to Center:         ") + (
            m_model->getWorld()->getBestTimeToCenter().getSeconds() < 0 ? "NONE" :
            SimUtilities::formatDuration(m_model->getWorld()->getBestTimeToCenter())
        ),

        // Sim state
        QString("Crashed:                     ") + (S()->crashed() ? "TRUE" : "FALSE"),
        QString("Layout Type (l):             ") + LAYOUT_TYPE_TO_STRING.value(S()->layoutType()),
        QString("Rotate Zoomed Map (r):       ") + (S()->rotateZoomedMap() ? "TRUE" : "FALSE"),
        QString("Zoomed Map Scale (i, o):     ") + QString::number(S()->zoomedMapScale()),
        QString("Wall Truth Visible (t):      ") + (S()->wallTruthVisible() ? "TRUE" : "FALSE"),
        QString("Tile Colors Visible (c):     ") + (S()->tileColorsVisible() ? "TRUE" : "FALSE"),
        QString("Tile Fog Visible (g):        ") + (S()->tileFogVisible() ? "TRUE" : "FALSE"),
        QString("Tile Text Visible (x):       ") + (S()->tileTextVisible() ? "TRUE" : "FALSE"),
        QString("Tile Distance Visible (d):   ") + (S()->tileDistanceVisible() ? "TRUE" : "FALSE"),
        QString("Header Visible (h):          ") + (S()->headerVisible() ? "TRUE" : "FALSE"),
        QString("Wireframe Mode (w):          ") + (S()->wireframeMode() ? "TRUE" : "FALSE"),
        QString("Paused (p):                  ") + (S()->paused() ? "TRUE" : "FALSE"),
        QString("Sim Speed (f, s):            ") + QString::number(S()->simSpeed()),
    };
}

void Header::updateColumnStartingPositions() {

    // This uses:
    // - m_windowWidth
    // - m_lines
    // - m_columnSpacing

    // TODO: MACK
    // static auto willFit = [&](const QVector<double>& columnWidths) {
    //     double sum = 0.0;
    //     for (double width : columnWidths) {
    //         sum += width;
    //     }
    //     sum += 2 * P()->windowBorderWidth();
    //     sum += (columnWidths.size() - 1) * m_columnSpacing;
    //     return sum <= m_windowWidth;
    // };

    // static auto getColumnWidths = [&](int numCols) {
    //     QVector<double> columnWidths;
    //     int numRows = getNumRows(m_lines.size(), numCols);
    //     for (int i = 0; i < numCols; i += 1) {
    //         double maxLineWidth = 0.0;
    //         for (int j = 0; j < numRows && i * numRows + j < m_lines.size(); j += 1) {
    //             double lineWidth = m_textDrawer->getWidth(m_lines.at(i * numRows + j));
    //             if (maxLineWidth < lineWidth) {
    //                 maxLineWidth = lineWidth;
    //             }
    //         }
    //         columnWidths.push_back(maxLineWidth);
    //     }
    //     return columnWidths;
    // };

    // // Determine the optimal column widths
    // QVector<double> columnWidths;
    // int numCols = 1;
    // do {
    //     columnWidths = getColumnWidths(numCols);
    //     numCols += 1;
    // }
    // while (willFit(getColumnWidths(numCols)));

    // // Calculate the starting positions based on the column widths
    // QVector<int> columnStartingPositions;
    // int currentColumnStart = P()->windowBorderWidth();
    // for (int i = 0; i < columnWidths.size(); i += 1) {
    //     columnStartingPositions.push_back(currentColumnStart);
    //     currentColumnStart += columnWidths.at(i) + m_columnSpacing;
    // }

    // // Actually update the member variable
    // m_columnStartingPositions = columnStartingPositions;
}

} // namespace mms
