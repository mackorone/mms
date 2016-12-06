#pragma once

#include <QKeyEvent>
#include <QMainWindow>

#include "Lens.h"
#include "Map.h"

namespace Ui {
class MainWindow;
}

namespace mms {

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(Lens* lens, Map* map, QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    Lens* m_lens;
    Ui::MainWindow *ui;

};

} // namespace mms
