#include "MainWindow.h"
#include "ui_mainwindow.h"

// TODO: MACK
#include "Example.h"

MainWindow::MainWindow(mms::View* view, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    //ui->m_openGlWidgetContainer->addWidget(view);
    mms::Example* e = new mms::Example(this);
    ui->m_openGlWidgetContainer->addWidget(e);
}

MainWindow::~MainWindow() {
    delete ui;
}
