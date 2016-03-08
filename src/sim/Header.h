#pragma once

#include "Model.h"
#include "TextDrawer.h"

namespace sim {

class Header {

public:
    Header(Model* model);
    int getHeight() const;
    void setMaxHeight(int maxHeight);
    void draw(int windowWidth, int windowHeight);

private:
    Model* m_model;
    int m_maxHeight;
    int m_textHeight;
    TextDrawer* m_textDrawer;

    std::vector<int> getColumnStartingPositions(
        int windowWidth, const std::vector<std::string>& lines, int border, int columnSeparation);

    // TODO: MACK Store the persistent strings as constants

};

} // namespace sim
