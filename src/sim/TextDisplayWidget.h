#pragma once

#include <QCheckBox>
#include <QPlainTextEdit>
#include <QPushButton>

namespace mms {

class TextDisplayWidget : public QWidget {

    Q_OBJECT

public:

    TextDisplayWidget();

    QPushButton* clearButton;
    QCheckBox* autoClearCheckBox;
    QPlainTextEdit* textEdit;

};

} // namespace mms
