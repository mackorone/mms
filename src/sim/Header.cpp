#include "Header.h"

#include "Layout.h"
#include "Param.h"
#include "SimUtilities.h"
#include "State.h"

namespace sim {

// TODO: MACK - make the text height a parameter
Header::Header(Model* model) : m_maxHeight(0), m_model(model), m_textHeight(10) {
    // TODO: MACK - If the font doesn't exist, we silently fail and draw no text whatsoever
    // Initialize the text drawer object // TODO: MACK - get the font from param
    m_textDrawer = new TextDrawer("Hack-Regular.ttf", m_textHeight);
}

int Header::getHeight() const {
    // TODO: MACK Do some calculations here, do max height, kill defaultHeaderHeight
    // return m_maxHeight;
    return std::min(m_maxHeight, P()->defaultHeaderHeight());
}

void Header::setMaxHeight(int maxHeight) {
    m_maxHeight = maxHeight;
}

void Header::draw(int windowWidth, int windowHeight) {

    int border = 5;
    m_textDrawer->commenceDrawingTextForFrame();
    // TODO: MACK - some kind of border here
    // TODO: MACK - put these in a data structure, cut of the text if necessary
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 1, windowWidth, windowHeight,
        std::string("Run ID: ") + S()->runId());
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 2, windowWidth, windowHeight,
        std::string("Crashed: ") + (S()->crashed() ? "TRUE" : "FALSE"));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 3, windowWidth, windowHeight,
        std::string("Layout Type (l): ") + LAYOUT_TYPE_TO_STRING.at(S()->layoutType()));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 4, windowWidth, windowHeight,
        std::string("Rotate Zoomed Map (r): ") + (S()->rotateZoomedMap() ? "TRUE" : "FALSE"));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 5, windowWidth, windowHeight,
        std::string("Zoomed Map Scale (i, o): ") + std::to_string(S()->zoomedMapScale()));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 6, windowWidth, windowHeight,
        std::string("Wall Truth Visible (t): ") + (S()->wallTruthVisible() ? "TRUE" : "FALSE"));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 7, windowWidth, windowHeight,
        std::string("Tile Colors Visible (c): ") + (S()->tileColorsVisible() ? "TRUE" : "FALSE"));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 8, windowWidth, windowHeight,
        std::string("Tile Fog Visible (g): ") + (S()->tileFogVisible() ? "TRUE" : "FALSE"));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 9, windowWidth, windowHeight,
        std::string("Tile Text Visible (x): ") + (S()->tileTextVisible() ? "TRUE" : "FALSE"));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 10, windowWidth, windowHeight,
        std::string("Tile Distance Visible (d): ") + (S()->tileDistanceVisible() ? "TRUE" : "FALSE"));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 11, windowWidth, windowHeight,
        std::string("Wireframe Mode (w): ") + (S()->wireframeMode() ? "TRUE" : "FALSE"));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 12, windowWidth, windowHeight,
        std::string("Paused (p): ") + (S()->paused() ? "TRUE" : "FALSE"));
    m_textDrawer->drawText(border, windowHeight - (m_textHeight + border) * 13, windowWidth, windowHeight,
        std::string("Sim Speed (f, s): ") + std::to_string(S()->simSpeed()));

    m_textDrawer->drawText(600, windowHeight - (m_textHeight + border) * 2, windowWidth, windowHeight,
        std::string("Elapsed Sim Time: ") + SimUtilities::formatSeconds(m_model->getMouse()->getElapsedSimTime().getSeconds()));
    m_textDrawer->drawText(600, windowHeight - (m_textHeight + border) * 4, windowWidth, windowHeight,
        std::string("Current X (m): ") + std::to_string(m_model->getMouse()->getCurrentTranslation().getX().getMeters()));
    m_textDrawer->drawText(600, windowHeight - (m_textHeight + border) * 5, windowWidth, windowHeight,
        std::string("Current Y (m): ") + std::to_string(m_model->getMouse()->getCurrentTranslation().getY().getMeters()));
    m_textDrawer->drawText(600, windowHeight - (m_textHeight + border) * 6, windowWidth, windowHeight,
        std::string("Current Rotation (deg): ") + std::to_string(m_model->getMouse()->getCurrentRotation().getDegreesZeroTo360()));
    m_textDrawer->drawText(600, windowHeight - (m_textHeight + border) * 8, windowWidth, windowHeight,
        std::string("Current X tile: ") + std::to_string(m_model->getMouse()->getCurrentDiscretizedTranslation().first));
    m_textDrawer->drawText(600, windowHeight - (m_textHeight + border) * 9, windowWidth, windowHeight,
        std::string("Current Y tile: ") + std::to_string(m_model->getMouse()->getCurrentDiscretizedTranslation().second));
    m_textDrawer->drawText(600, windowHeight - (m_textHeight + border) * 10, windowWidth, windowHeight,
        std::string("Current Direction: ") + DIRECTION_TO_STRING.at(m_model->getMouse()->getCurrentDiscretizedRotation()));

    m_textDrawer->concludeDrawingTextForFrame();
}

} // namespace sim
