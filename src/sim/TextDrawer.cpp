#include "TextDrawer.h"

#include "Assert.h"
#include "Directory.h"
#include "GraphicUtilities.h"

namespace sim {

TextDrawer* TextDrawer::m_activeTextDrawer = nullptr;
TextDrawer::TextDrawer(const std::string& font, float size) :
    m_stash(sth_create(512, 512)),
    m_font(sth_add_font(m_stash, (Directory::getResFontsDirectory() + font).c_str())),
    m_size(size) {
}

void TextDrawer::commenceDrawingTextForFrame() {
    sth_begin_draw(m_stash);
    m_activeTextDrawer = this;
}

void TextDrawer::drawText(float x, float y, const std::string& str) {
    ASSERT_EQ(m_activeTextDrawer, this);
    glLoadIdentity();
    std::pair<int, int> windowSize = GraphicUtilities::getWindowSize();
    glOrtho(0, windowSize.first, 0, windowSize.second, -1, 1);
    // TODO: upforgrabs
    // Right now, we use a hack to draw the text approximately "size" pixels
    // tall: simply multiply the given value by 1.6. It'd be nice to figure out
    // *why* we need to do this (i.e., investigate both fontstash and
    // stb_truetype) and to supply a proper fix (as opposed to using the hack).
    sth_draw_text(m_stash, m_font, m_size * 1.6f, x, y, str.c_str(), nullptr);
}

void TextDrawer::concludeDrawingTextForFrame() {
    sth_end_draw(m_stash);
    m_activeTextDrawer = nullptr;
}

} // namespace sim
