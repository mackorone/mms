/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *mapContainer;
    QTabWidget *tabs;
    QWidget *runStatsTab;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *runStatsScrollArea;
    QWidget *runStatsScrollAreaContents;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *runStatsLayout;
    QWidget *stdoutTab;
    QVBoxLayout *verticalLayout_4;
    QScrollArea *stdoutScrollArea;
    QWidget *stdoutScrollAreaContents;
    QVBoxLayout *verticalLayout_7;
    QPlainTextEdit *stdoutTextEdit;
    QWidget *algoOptionsTab;
    QVBoxLayout *verticalLayout_6;
    QWidget *apiCallsTab;
    QVBoxLayout *verticalLayout_5;
    QWidget *mazeInfoTab;
    QVBoxLayout *verticalLayout;
    QScrollArea *mazeInfoScrollArea;
    QWidget *mazeInfoScrollAreaContents;
    QVBoxLayout *verticalLayout_8;
    QGridLayout *mazeInfoLayout;
    QWidget *optionsTab;
    QVBoxLayout *verticalLayout_9;
    QScrollArea *optionsScrollArea;
    QWidget *optionsScrollAreaContents;
    QVBoxLayout *verticalLayout_10;
    QGridLayout *optionsLayout;
    QMenuBar *menuBar;
    QMenu *menu_File;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(640, 480);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(centralWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        mapContainer = new QVBoxLayout();
        mapContainer->setSpacing(6);
        mapContainer->setObjectName(QStringLiteral("mapContainer"));

        horizontalLayout_3->addLayout(mapContainer);

        tabs = new QTabWidget(centralWidget);
        tabs->setObjectName(QStringLiteral("tabs"));
        runStatsTab = new QWidget();
        runStatsTab->setObjectName(QStringLiteral("runStatsTab"));
        verticalLayout_3 = new QVBoxLayout(runStatsTab);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        runStatsScrollArea = new QScrollArea(runStatsTab);
        runStatsScrollArea->setObjectName(QStringLiteral("runStatsScrollArea"));
        runStatsScrollArea->setWidgetResizable(true);
        runStatsScrollAreaContents = new QWidget();
        runStatsScrollAreaContents->setObjectName(QStringLiteral("runStatsScrollAreaContents"));
        runStatsScrollAreaContents->setGeometry(QRect(0, 0, 590, 384));
        verticalLayout_2 = new QVBoxLayout(runStatsScrollAreaContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        runStatsLayout = new QGridLayout();
        runStatsLayout->setSpacing(6);
        runStatsLayout->setObjectName(QStringLiteral("runStatsLayout"));

        verticalLayout_2->addLayout(runStatsLayout);

        runStatsScrollArea->setWidget(runStatsScrollAreaContents);

        verticalLayout_3->addWidget(runStatsScrollArea);

        tabs->addTab(runStatsTab, QString());
        stdoutTab = new QWidget();
        stdoutTab->setObjectName(QStringLiteral("stdoutTab"));
        verticalLayout_4 = new QVBoxLayout(stdoutTab);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        stdoutScrollArea = new QScrollArea(stdoutTab);
        stdoutScrollArea->setObjectName(QStringLiteral("stdoutScrollArea"));
        stdoutScrollArea->setWidgetResizable(true);
        stdoutScrollAreaContents = new QWidget();
        stdoutScrollAreaContents->setObjectName(QStringLiteral("stdoutScrollAreaContents"));
        stdoutScrollAreaContents->setGeometry(QRect(0, 0, 590, 384));
        verticalLayout_7 = new QVBoxLayout(stdoutScrollAreaContents);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        stdoutTextEdit = new QPlainTextEdit(stdoutScrollAreaContents);
        stdoutTextEdit->setObjectName(QStringLiteral("stdoutTextEdit"));
        stdoutTextEdit->setReadOnly(true);
        stdoutTextEdit->setTextInteractionFlags(Qt::TextSelectableByMouse);

        verticalLayout_7->addWidget(stdoutTextEdit);

        stdoutScrollArea->setWidget(stdoutScrollAreaContents);

        verticalLayout_4->addWidget(stdoutScrollArea);

        tabs->addTab(stdoutTab, QString());
        algoOptionsTab = new QWidget();
        algoOptionsTab->setObjectName(QStringLiteral("algoOptionsTab"));
        verticalLayout_6 = new QVBoxLayout(algoOptionsTab);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        tabs->addTab(algoOptionsTab, QString());
        apiCallsTab = new QWidget();
        apiCallsTab->setObjectName(QStringLiteral("apiCallsTab"));
        verticalLayout_5 = new QVBoxLayout(apiCallsTab);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        tabs->addTab(apiCallsTab, QString());
        mazeInfoTab = new QWidget();
        mazeInfoTab->setObjectName(QStringLiteral("mazeInfoTab"));
        verticalLayout = new QVBoxLayout(mazeInfoTab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        mazeInfoScrollArea = new QScrollArea(mazeInfoTab);
        mazeInfoScrollArea->setObjectName(QStringLiteral("mazeInfoScrollArea"));
        mazeInfoScrollArea->setWidgetResizable(true);
        mazeInfoScrollAreaContents = new QWidget();
        mazeInfoScrollAreaContents->setObjectName(QStringLiteral("mazeInfoScrollAreaContents"));
        mazeInfoScrollAreaContents->setGeometry(QRect(0, 0, 590, 384));
        verticalLayout_8 = new QVBoxLayout(mazeInfoScrollAreaContents);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        mazeInfoLayout = new QGridLayout();
        mazeInfoLayout->setSpacing(6);
        mazeInfoLayout->setObjectName(QStringLiteral("mazeInfoLayout"));

        verticalLayout_8->addLayout(mazeInfoLayout);

        mazeInfoScrollArea->setWidget(mazeInfoScrollAreaContents);

        verticalLayout->addWidget(mazeInfoScrollArea);

        tabs->addTab(mazeInfoTab, QString());
        optionsTab = new QWidget();
        optionsTab->setObjectName(QStringLiteral("optionsTab"));
        verticalLayout_9 = new QVBoxLayout(optionsTab);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        optionsScrollArea = new QScrollArea(optionsTab);
        optionsScrollArea->setObjectName(QStringLiteral("optionsScrollArea"));
        optionsScrollArea->setWidgetResizable(true);
        optionsScrollAreaContents = new QWidget();
        optionsScrollAreaContents->setObjectName(QStringLiteral("optionsScrollAreaContents"));
        optionsScrollAreaContents->setGeometry(QRect(0, 0, 590, 384));
        verticalLayout_10 = new QVBoxLayout(optionsScrollAreaContents);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        optionsLayout = new QGridLayout();
        optionsLayout->setSpacing(6);
        optionsLayout->setObjectName(QStringLiteral("optionsLayout"));

        verticalLayout_10->addLayout(optionsLayout);

        optionsScrollArea->setWidget(optionsScrollAreaContents);

        verticalLayout_9->addWidget(optionsScrollArea);

        tabs->addTab(optionsTab, QString());

        horizontalLayout_3->addWidget(tabs);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 25));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_File->menuAction());

        retranslateUi(MainWindow);

        tabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Micromouse Simulator", 0));
        tabs->setTabText(tabs->indexOf(runStatsTab), QApplication::translate("MainWindow", "Run Stats", 0));
        stdoutTextEdit->setPlainText(QString());
        tabs->setTabText(tabs->indexOf(stdoutTab), QApplication::translate("MainWindow", "Algo Stdout", 0));
        tabs->setTabText(tabs->indexOf(algoOptionsTab), QApplication::translate("MainWindow", "Algo Options", 0));
        tabs->setTabText(tabs->indexOf(apiCallsTab), QApplication::translate("MainWindow", "API Calls", 0));
        tabs->setTabText(tabs->indexOf(mazeInfoTab), QApplication::translate("MainWindow", "Maze Info", 0));
        tabs->setTabText(tabs->indexOf(optionsTab), QApplication::translate("MainWindow", "Options", 0));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
