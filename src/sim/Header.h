#pragma once

#include "Model.h"
#include "TextDrawer.h"

namespace sim {

class Header {

public:
    Header(Model* model);
    int getHeight() const;
    void updateWindowSize(int width, int height);
    void draw();

private:
    Model* m_model;
    int m_windowWidth;
    int m_windowHeight;
    int m_headerHeight;
    int m_textHeight;
    TextDrawer* m_textDrawer;

    // TODO: MACK
    int calculateHeaderHeight(int windowWidth, int windowHeight);

    // TODO: MACK - make this stateful about the window width and height, only calculate things if those change
    std::vector<std::string> getLines() const;

    std::vector<int> getColumnStartingPositions(
        int windowWidth, const std::vector<std::string>& lines, int border, int columnSeparation) const;

    // TODO: MACK Store the persistent strings as constants

};

} // namespace sim
