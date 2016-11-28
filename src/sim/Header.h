#pragma once

#include "Model.h"
#include "StaticMouseAlgorithmOptions.h"
#include "World.h"

// TODO: MACK
// #include "TextDrawer.h"

namespace mms {

// We have to forward declare the class (as opposed to including it) so as to
// avoid a circular dependency; ControllerManager.h already includes this file
class ControllerManager;

class Header {

public:
    Header(Model* model);
    int getHeight() const;
    void setControllerManager(ControllerManager* controllerManager);
    void updateWindowSize(int width, int height);
    void updateLinesAndColumnStartingPositions();
    void draw();

private:
    Model* m_model;
    ControllerManager* m_controllerManager; // Used to retrieve mouse algo options
    int m_windowWidth; // The total height of the window, in pixels
    int m_windowHeight; // The total width of the window, in pixels
    int m_textHeight; // The height of the text, in pixels
    int m_rowSpacing; // The space between rows, in pixels
    int m_columnSpacing; // The space between columns, in pixels
    QVector<int> m_columnStartingPositions; // The starting horizontal position of each column, in pixels
    QVector<QString> m_lines; // The lines of text that we're drawing in the header
    // TODO: MACK
    // TextDrawer* m_textDrawer; // The object used to dimension and draw the header text

    // Return the number of rows of text that to be displayed
    int getNumRows(int numLines, int numCols) const;

    // Update the lines of text to be drawn in the header
    void updateLines();

    // Update the column starting positions
    void updateColumnStartingPositions();

};

} // namespace mms
