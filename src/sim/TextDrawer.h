#pragma once

#include <fontstash/fontstash.h>
#include <string>

#include "ViewData.h" // TODO: MACK

namespace sim {

class TextDrawer {

    // This is a convenience class for drawing (relatively) small amounts of
    // dynamic horizontal text via fontstash. It is not suitable to tile text,
    // which is why tile text was instead implemented using bitmap textures.

public:

    // Note that the TextDrawer may only be instantiated after
    // glutInit is called. Thus, you may not declare it statically.
    TextDrawer(const std::string& font, float size, ViewData* data); // TODO: MACK

    // Using the class is as easy as calling these methods, in order, during
    // the rendering loop. Commence and conclude should only be called once per
    // frame, but note that drawText may be called many times per frame.
    void commenceDrawingTextForFrame();
    void drawText(float x, float y, const std::string& str);
    void concludeDrawingTextForFrame();

private:
    sth_stash* m_stash;
    int m_font;
    float m_size;

    ViewData* m_data; // TODO: MACK

    // Only one TextDrawer be active at a time. We use a static
    // class member to ensure that this requirement is upheld.
    static TextDrawer* m_activeTextDrawer;
};

} // namespace sim
