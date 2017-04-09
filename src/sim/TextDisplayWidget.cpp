#include "TextDisplayWidget.h"

#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace mms {

TextDisplayWidget::TextDisplayWidget() :
        clearButton(new QPushButton("Clear")),
        autoClearCheckBox(new QCheckBox("Auto-clear")),
        textEdit(new QPlainTextEdit()) {

    // Configure sub-widgets
    connect(clearButton, &QPushButton::clicked, this, [=](){
        textEdit->clear();
    });
    // TODO: upforgrabs
    // The checked-ness should be saved and re-loaded
    autoClearCheckBox->setCheckState(Qt::Checked);
    textEdit->setReadOnly(true);
    // TODO: upforgrabs
    // The line wrap mode should be configurable
    textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    // TODO: upforgrabs
    // This font size should be configurable
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(10);
    textEdit->document()->setDefaultFont(font);

    // Arrange the widgets
    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->addWidget(clearButton);
    headerLayout->addWidget(autoClearCheckBox);
    headerLayout->addSpacerItem(
        new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed)
    );
    layout->addLayout(headerLayout);
    layout->addWidget(textEdit);
    setLayout(layout);
}

} // namespace mms
