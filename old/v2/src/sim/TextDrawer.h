#pragma once

#include <fontstash/fontstash.h>
#include <string>

namespace sim {

class TextDrawer {

    // This is a convenience class for drawing (relatively) small amounts of
    // dynamic horizontal text via fontstash. It is not suitable to tile text,
    // which is why tile text was instead implemented using bitmap textures.

public:

    // Note that the TextDrawer may only be instantiated after
    // glutInit is called. Thus, you may not declare it statically.
    TextDrawer(const std::string& fontPath, float size);

    // Using the class is as easy as calling these methods, in order, during
    // the rendering loop. Commence and conclude should only be called once per
    // frame, but note that drawText and getWidth may be called many times per
    // frame.
    void commenceDrawingTextForFrame();
    float getWidth(const std::string& str);
    void drawText(float x, float y, int windowWidth, int windowHeight, const std::string& str);
    void concludeDrawingTextForFrame();

private:
    sth_stash* m_stash;
    int m_font;
    float m_size;

    // TODO: upforgrabs
    // Right now, we use a hack to draw the text approximately "size" pixels
    // tall: simply multiply the given value by 1.6. It'd be nice to figure out
    // *why* we need to do this (i.e., investigate both fontstash and
    // stb_truetype) and to supply a proper fix (as opposed to using the hack).
    static const float SCALE_FACTOR;

    // Only one TextDrawer be active at a time. We use a static
    // class member to ensure that this requirement is upheld.
    static TextDrawer* m_activeTextDrawer;
};

} // namespace sim
