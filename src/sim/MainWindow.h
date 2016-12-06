#pragma once

#include <QKeyEvent>
#include <QMainWindow>

// TODO: MACK - rename this, it can just be class declaration
#include "Map.h"

namespace Ui {
class MainWindow;
}

namespace mms {

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(Map* map, QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    Map* m_map;
    Ui::MainWindow *ui;

};

} // namespace mms
