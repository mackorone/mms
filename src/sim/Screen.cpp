#include "Screen.h"

#include <QApplication>
#include <QDesktopWidget>

#include "Assert.h"

namespace mms {

Screen* Screen::INSTANCE = nullptr;

void Screen::init() {
    ASSERT_TR(INSTANCE == nullptr);
    INSTANCE = new Screen();
}

Screen* Screen::get() {
    ASSERT_FA(INSTANCE == nullptr);
    return INSTANCE;
}

double Screen::pixelsPerMeter() {
    return m_pixelsPerMeter;
}

Screen::Screen() {
    QDesktopWidget* widget = QApplication::desktop();
    int pixels = widget->availableGeometry().width();
    double meters = widget->widthMM() / 1000.0;
    m_pixelsPerMeter = pixels / meters;
}

} // namespace mms
