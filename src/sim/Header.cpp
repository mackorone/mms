#include "Header.h"

#include "Layout.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"

#include "Logging.h" // TODO: MACK

namespace sim {

// TODO: MACK - make the text height a parameter
Header::Header(Model* model) : m_model(model), m_windowWidth(0), m_windowHeight(0), m_headerHeight(0), m_textHeight(10) {
    // TODO: MACK - If the font doesn't exist, we silently fail and draw no text whatsoever
    // Initialize the text drawer object // TODO: MACK - get the font from param
    m_textDrawer = new TextDrawer("Hack-Regular.ttf", m_textHeight);
}

int Header::getHeight() const {
    // TODO: Kill P()->defaultheaderHeight()
    return m_headerHeight;
}

void Header::updateWindowSize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
    m_headerHeight = calculateHeaderHeight(width, height);
}

void Header::draw() {

    // TODO: MACK - it's expensive to create this vector every single frame

    std::vector<std::string> lines = getLines();

    // TODO: MACK add the ellipses here..
    // TODO: MACK - some kind of border here
    // TODO: MACK - put these in a data structure, cut of the text if necessary
    int border = 5;
    int columnSeparation = 20;

    int windowWidth = m_windowWidth;
    int windowHeight = m_windowHeight;

    std::vector<int> startingColumnPositions = getColumnStartingPositions(windowWidth, lines, border, columnSeparation);
    int linesPerColumn = lines.size() / startingColumnPositions.size();

    m_textDrawer->commenceDrawingTextForFrame();
    for (int i = 0; i < startingColumnPositions.size(); i += 1) {
        for (int j = 0; j < linesPerColumn && i * linesPerColumn + j < lines.size(); j += 1) {
            m_textDrawer->drawText(
                startingColumnPositions.at(i),
                windowHeight - (m_textHeight + border) * (j + 1),
                windowWidth,
                windowHeight,
                lines.at(i * linesPerColumn + j)
            );
        }
    }
    m_textDrawer->concludeDrawingTextForFrame();
}

int Header::calculateHeaderHeight(int windowWidth, int windowHeight) {
    int border = 5;
    int columnSeparation = 20;
    std::vector<std::string> lines = getLines();
    int rows = lines.size() / getColumnStartingPositions(windowWidth, lines, border, columnSeparation).size();
    int size = rows * m_textHeight + (rows + 1) * border; // TODO: MACK this is a row separator
    return size;
}

std::vector<std::string> Header::getLines() const {
// TODO: MACK - if the lines change without the window size changing, the text state could become bad...

    return {
        std::string("Run ID: ") + S()->runId(),
        std::string("Crashed: ") + (S()->crashed() ? "TRUE" : "FALSE"),
        std::string("Layout Type (l): ") + LAYOUT_TYPE_TO_STRING.at(S()->layoutType()),
        std::string("Rotate Zoomed Map (r): ") + (S()->rotateZoomedMap() ? "TRUE" : "FALSE"),
        std::string("Zoomed Map Scale (i, o): ") + std::to_string(S()->zoomedMapScale()),
        std::string("Wall Truth Visible (t): ") + (S()->wallTruthVisible() ? "TRUE" : "FALSE"),
        std::string("Tile Colors Visible (c): ") + (S()->tileColorsVisible() ? "TRUE" : "FALSE"),
        std::string("Tile Fog Visible (g): ") + (S()->tileFogVisible() ? "TRUE" : "FALSE"),
        std::string("Tile Text Visible (x): ") + (S()->tileTextVisible() ? "TRUE" : "FALSE"),
        std::string("Tile Distance Visible (d): ") + (S()->tileDistanceVisible() ? "TRUE" : "FALSE"),
        std::string("Wireframe Mode (w): ") + (S()->wireframeMode() ? "TRUE" : "FALSE"),
        std::string("Paused (p): ") + (S()->paused() ? "TRUE" : "FALSE"),
        std::string("Sim Speed (f, s): ") + std::to_string(S()->simSpeed()),
        std::string("Elapsed Sim Time: ") + SimUtilities::formatSeconds(m_model->getMouse()->getElapsedSimTime().getSeconds()),
        std::string("Current X (m): ") + std::to_string(m_model->getMouse()->getCurrentTranslation().getX().getMeters()),
        std::string("Current Y (m): ") + std::to_string(m_model->getMouse()->getCurrentTranslation().getY().getMeters()),
        std::string("Current Rotation (deg): ") + std::to_string(m_model->getMouse()->getCurrentRotation().getDegreesZeroTo360()),
        std::string("Current X tile: ") + std::to_string(m_model->getMouse()->getCurrentDiscretizedTranslation().first),
        std::string("Current Y tile: ") + std::to_string(m_model->getMouse()->getCurrentDiscretizedTranslation().second),
        std::string("Current Direction: ") + DIRECTION_TO_STRING.at(m_model->getMouse()->getCurrentDiscretizedRotation()),
    };
}

std::vector<int> Header::getColumnStartingPositions(
        int windowWidth, const std::vector<std::string>& lines, int border, int columnSeparation) const {

    static auto willFit = [=](const std::vector<double>& columnWidths, int windowWidth, int columnSeparation) {
        double sum = 0.0;
        for (double width : columnWidths) {
            sum += width;
        }
        sum += 2 * border;
        sum += (columnWidths.size() - 1) * columnSeparation;
        return sum <= windowWidth;
    };

    static auto getColumnWidths = [=](const std::vector<std::string>& lines, int numCols) {
        std::vector<double> columnWidths;
        int linesPerColumn = lines.size() / numCols;
        for (int i = 0; i < numCols; i += 1) {
            double maxLineWidth = 0.0;
            for (int j = 0; j < linesPerColumn && i * linesPerColumn + j < lines.size(); j += 1) {
                double lineWidth = m_textDrawer->getWidth(lines.at(i * linesPerColumn + j));
                if (maxLineWidth < lineWidth) {
                    maxLineWidth = lineWidth;
                }
            }
            columnWidths.push_back(maxLineWidth);
        }
        return columnWidths;
    };

    // Determine the optimal column widths
    std::vector<double> columnWidths;
    int numCols = 1;
    do {
        columnWidths = getColumnWidths(lines, numCols);
        numCols += 1;
    }
    while (willFit(getColumnWidths(lines, numCols), windowWidth, columnSeparation));

    // Calculate the starting positions based on the column widths
    std::vector<int> columnStartingPositions;
    int currentColumnStart = border;
    for (int i = 0; i < columnWidths.size(); i += 1) {
        columnStartingPositions.push_back(currentColumnStart);
        currentColumnStart += columnWidths.at(i) + columnSeparation;
    }
    return columnStartingPositions;
}

} // namespace sim
