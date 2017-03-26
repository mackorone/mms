#include "TextDisplay.h"

#include <QFont>

namespace mms {

TextDisplay::TextDisplay() {
    setReadOnly(true);
    // TODO: upforgrabs
    // The line wrap mode should be configurable
    setLineWrapMode(QPlainTextEdit::NoWrap);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    // TODO: upforgrabs
    // This font size should be configurable
    font.setPointSize(10);
    document()->setDefaultFont(font);
}

} // namespace mms
