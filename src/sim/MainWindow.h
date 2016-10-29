#pragma once

#include <QMainWindow>

// TODO: MACK - rename this, it can just be class declaration
#include "View.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(mms::View* view, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

};
