#pragma once

#include <fontstash/fontstash.h>
#include <string>

namespace sim {

class TextDrawer {

public:
    TextDrawer(const std::string& font, float pt);
    void commenceDrawingTextForFrame();
    void drawText(float x, float y, const std::string& str);
    void concludeDrawingTextForFrame();

private:
    struct sth_stash* m_stash;
    int m_font;
    float m_pt;

    // Only one text drawer should be active at a time. We use a static
    // class member to ensure tha tthis requirement is upheld.
    static TextDrawer* m_activeTextDrawer;
};

} // namespace sim
