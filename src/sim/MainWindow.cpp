#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(mms::View* view, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->m_openGlWidgetContainer->addWidget(view);
}

MainWindow::~MainWindow() {
    delete ui;
}
