#pragma once

#include <QKeyEvent>
#include <QMainWindow>

#include "Lens.h"
#include "Model.h"

namespace Ui {
class MainWindow;
}

namespace mms {

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(Model* model, Lens* lens, QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    Model* m_model;
    Lens* m_lens;
    Ui::MainWindow *ui;

};

} // namespace mms
