#pragma once

#include "Model.h"
#include "StaticMouseAlgorithmOptions.h"
#include "TextDrawer.h"
#include "World.h"

class IMouseAlgorithm;

namespace sim {

class Header {

public:
    Header(Model* model);
    int getHeight() const;
    void setMouseAlgorithmAndOptions(
        IMouseAlgorithm* mouseAlgorithm,
        StaticMouseAlgorithmOptions options);
    void updateWindowSize(int width, int height);
    void updateLinesAndColumnStartingPositions();
    void draw();

private:
    Model* m_model;
    IMouseAlgorithm* m_mouseAlgorithm; // Used to retrieve dynamic mouse algo options
    StaticMouseAlgorithmOptions m_options; // The options whose values don't change
    int m_windowWidth; // The total height of the window, in pixels
    int m_windowHeight; // The total width of the window, in pixels
    int m_textHeight; // The height of the text, in pixels
    int m_rowSpacing; // The space between rows, in pixels
    int m_columnSpacing; // The space between columns, in pixels
    std::vector<int> m_columnStartingPositions; // The starting horizontal position of each column, in pixels
    std::vector<std::string> m_lines; // The lines of text that we're drawing in the header
    TextDrawer* m_textDrawer; // The object used to dimension and draw the header text

    // Return the number of rows of text that to be displayed
    int getNumRows(int numLines, int numCols) const;

    // Update the lines of text to be drawn in the header
    void updateLines();

    // Update the column starting positions
    void updateColumnStartingPositions();

};

} // namespace sim
