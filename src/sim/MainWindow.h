#pragma once

#include <QKeyEvent>
#include <QMainWindow>

// TODO: MACK - rename this, it can just be class declaration
#include "View.h"

namespace Ui {
class MainWindow;
}

namespace mms {

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(mms::View* view, QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    View* m_view;
    Ui::MainWindow *ui;

};

} // namespace mms
