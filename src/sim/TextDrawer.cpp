#include "TextDrawer.h"

#include "Assert.h"

namespace sim {

const float TextDrawer::SCALE_FACTOR = 1.6;

TextDrawer* TextDrawer::m_activeTextDrawer = nullptr;
TextDrawer::TextDrawer(const QString& fontPath, float size) :
        m_stash(sth_create(512, 512)),
        m_font(sth_add_font(m_stash, fontPath.toStdString().c_str())),
        m_size(size) {
}

void TextDrawer::commenceDrawingTextForFrame() {
    sth_begin_draw(m_stash);
    m_activeTextDrawer = this;
}

float TextDrawer::getWidth(const QString& str) {
    static float minX = 0.0;
    static float minY = 0.0;
    static float maxX = 0.0;
    static float maxY = 0.0;
    sth_dim_text(
        m_stash,
        m_font,
        m_size * SCALE_FACTOR,
        str.toStdString().c_str(),
        &minX,
        &minY,
        &maxX,
        &maxY
    );
    return maxX - minX;
}

void TextDrawer::drawText(float x, float y, int windowWidth, int windowHeight, const QString& str) {
    SIM_ASSERT_EQ(m_activeTextDrawer, this);
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
    sth_draw_text(
        m_stash,
        m_font,
        m_size * SCALE_FACTOR,
        x,
        y,
        str.toStdString().c_str(),
        nullptr
    );
}

void TextDrawer::concludeDrawingTextForFrame() {
    sth_end_draw(m_stash);
    m_activeTextDrawer = nullptr;
}

} // namespace sim
