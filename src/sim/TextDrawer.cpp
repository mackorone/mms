#include "TextDrawer.h"

#include "Assert.h"
#include "GraphicUtilities.h"
#include "SimUtilities.h"

namespace sim {

TextDrawer* TextDrawer::m_activeTextDrawer = nullptr;
TextDrawer::TextDrawer(const std::string& font, float pt) :
    m_stash(sth_create(512, 512)),
    m_font(sth_add_font(m_stash, (sim::SimUtilities::getProjectDirectory() + "res/fonts/" + font).c_str())),
    m_pt(pt) {
}

void TextDrawer::commenceDrawingTextForFrame() {
    sth_begin_draw(m_stash);
    m_activeTextDrawer = this;
}

void TextDrawer::drawText(float x, float y, const std::string& str) {
    ASSERT_EQUAL(m_activeTextDrawer, this);
    glLoadIdentity();
    std::pair<int, int> windowSize = GraphicUtilities::getWindowSize();
    glOrtho(0, windowSize.first, 0, windowSize.second, -1, 1);
    sth_draw_text(m_stash, m_font, m_pt, x, y, str.c_str(), nullptr);
}

void TextDrawer::concludeDrawingTextForFrame() {
    sth_end_draw(m_stash);
    m_activeTextDrawer = nullptr;
}

} // namespace sim
