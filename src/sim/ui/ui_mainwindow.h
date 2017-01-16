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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    QWidget *mapWidget;
    QVBoxLayout *mapLayout;
    QWidget *otherWidget;
    QVBoxLayout *controlLayout;
    QWidget *controlWidget;
    QGridLayout *gridLayout;
    QPushButton *runButton;
    QLabel *buildProgressLabel;
    QPushButton *buildButton;
    QLabel *selectAlgorithmLabel;
    QPushButton *buildAndRunButton;
    QProgressBar *buildProgressBar;
    QComboBox *selectAlgorithmComboBox;
    QWidget *infoWidget;
    QVBoxLayout *verticalLayout_13;
    QTabWidget *outputWidget;
    QWidget *buildTab;
    QVBoxLayout *verticalLayout_14;
    QScrollArea *buildScrollArea;
    QWidget *buildLayout;
    QVBoxLayout *verticalLayout_16;
    QPlainTextEdit *buildTextEdit;
    QWidget *runTab;
    QVBoxLayout *verticalLayout_15;
    QScrollArea *runScrollArea;
    QWidget *runLayout;
    QVBoxLayout *verticalLayout_17;
    QPlainTextEdit *runTextEdit;
    QTabWidget *statusWidget_2;
    QWidget *tab;
    QWidget *tab_2;
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
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(0);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(true);
        splitter->setHandleWidth(9);
        splitter->setChildrenCollapsible(false);
        mapWidget = new QWidget(splitter);
        mapWidget->setObjectName(QStringLiteral("mapWidget"));
        mapWidget->setMinimumSize(QSize(300, 300));
        mapLayout = new QVBoxLayout(mapWidget);
        mapLayout->setSpacing(6);
        mapLayout->setContentsMargins(11, 11, 11, 11);
        mapLayout->setObjectName(QStringLiteral("mapLayout"));
        mapLayout->setContentsMargins(-1, -1, 3, -1);
        splitter->addWidget(mapWidget);
        otherWidget = new QWidget(splitter);
        otherWidget->setObjectName(QStringLiteral("otherWidget"));
        otherWidget->setStyleSheet(QStringLiteral(""));
        controlLayout = new QVBoxLayout(otherWidget);
        controlLayout->setSpacing(0);
        controlLayout->setContentsMargins(11, 11, 11, 11);
        controlLayout->setObjectName(QStringLiteral("controlLayout"));
        controlLayout->setContentsMargins(0, 0, 0, 0);
        controlWidget = new QWidget(otherWidget);
        controlWidget->setObjectName(QStringLiteral("controlWidget"));
        gridLayout = new QGridLayout(controlWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(3, -1, -1, 6);
        runButton = new QPushButton(controlWidget);
        runButton->setObjectName(QStringLiteral("runButton"));

        gridLayout->addWidget(runButton, 2, 1, 1, 1);

        buildProgressLabel = new QLabel(controlWidget);
        buildProgressLabel->setObjectName(QStringLiteral("buildProgressLabel"));
        buildProgressLabel->setLayoutDirection(Qt::LeftToRight);
        buildProgressLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(buildProgressLabel, 1, 0, 1, 1);

        buildButton = new QPushButton(controlWidget);
        buildButton->setObjectName(QStringLiteral("buildButton"));

        gridLayout->addWidget(buildButton, 2, 0, 1, 1);

        selectAlgorithmLabel = new QLabel(controlWidget);
        selectAlgorithmLabel->setObjectName(QStringLiteral("selectAlgorithmLabel"));
        selectAlgorithmLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(selectAlgorithmLabel, 0, 0, 1, 1);

        buildAndRunButton = new QPushButton(controlWidget);
        buildAndRunButton->setObjectName(QStringLiteral("buildAndRunButton"));

        gridLayout->addWidget(buildAndRunButton, 2, 2, 1, 1);

        buildProgressBar = new QProgressBar(controlWidget);
        buildProgressBar->setObjectName(QStringLiteral("buildProgressBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buildProgressBar->sizePolicy().hasHeightForWidth());
        buildProgressBar->setSizePolicy(sizePolicy1);
        buildProgressBar->setValue(24);

        gridLayout->addWidget(buildProgressBar, 1, 1, 1, 2);

        selectAlgorithmComboBox = new QComboBox(controlWidget);
        selectAlgorithmComboBox->setObjectName(QStringLiteral("selectAlgorithmComboBox"));
        sizePolicy.setHeightForWidth(selectAlgorithmComboBox->sizePolicy().hasHeightForWidth());
        selectAlgorithmComboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(selectAlgorithmComboBox, 0, 1, 1, 2);


        controlLayout->addWidget(controlWidget);

        infoWidget = new QWidget(otherWidget);
        infoWidget->setObjectName(QStringLiteral("infoWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(infoWidget->sizePolicy().hasHeightForWidth());
        infoWidget->setSizePolicy(sizePolicy2);
        verticalLayout_13 = new QVBoxLayout(infoWidget);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        verticalLayout_13->setContentsMargins(3, 0, -1, -1);
        outputWidget = new QTabWidget(infoWidget);
        outputWidget->setObjectName(QStringLiteral("outputWidget"));
        sizePolicy.setHeightForWidth(outputWidget->sizePolicy().hasHeightForWidth());
        outputWidget->setSizePolicy(sizePolicy);
        buildTab = new QWidget();
        buildTab->setObjectName(QStringLiteral("buildTab"));
        verticalLayout_14 = new QVBoxLayout(buildTab);
        verticalLayout_14->setSpacing(3);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        verticalLayout_14->setContentsMargins(3, 3, 3, 3);
        buildScrollArea = new QScrollArea(buildTab);
        buildScrollArea->setObjectName(QStringLiteral("buildScrollArea"));
        sizePolicy.setHeightForWidth(buildScrollArea->sizePolicy().hasHeightForWidth());
        buildScrollArea->setSizePolicy(sizePolicy);
        buildScrollArea->setWidgetResizable(true);
        buildLayout = new QWidget();
        buildLayout->setObjectName(QStringLiteral("buildLayout"));
        buildLayout->setGeometry(QRect(0, 0, 307, 199));
        verticalLayout_16 = new QVBoxLayout(buildLayout);
        verticalLayout_16->setSpacing(3);
        verticalLayout_16->setContentsMargins(11, 11, 11, 11);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        verticalLayout_16->setContentsMargins(3, 3, 3, 3);
        buildTextEdit = new QPlainTextEdit(buildLayout);
        buildTextEdit->setObjectName(QStringLiteral("buildTextEdit"));
        sizePolicy.setHeightForWidth(buildTextEdit->sizePolicy().hasHeightForWidth());
        buildTextEdit->setSizePolicy(sizePolicy);
        buildTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        buildTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_16->addWidget(buildTextEdit);

        buildScrollArea->setWidget(buildLayout);

        verticalLayout_14->addWidget(buildScrollArea);

        outputWidget->addTab(buildTab, QString());
        buildScrollArea->raise();
        runTab = new QWidget();
        runTab->setObjectName(QStringLiteral("runTab"));
        verticalLayout_15 = new QVBoxLayout(runTab);
        verticalLayout_15->setSpacing(3);
        verticalLayout_15->setContentsMargins(11, 11, 11, 11);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        verticalLayout_15->setContentsMargins(3, 3, 3, 3);
        runScrollArea = new QScrollArea(runTab);
        runScrollArea->setObjectName(QStringLiteral("runScrollArea"));
        sizePolicy.setHeightForWidth(runScrollArea->sizePolicy().hasHeightForWidth());
        runScrollArea->setSizePolicy(sizePolicy);
        runScrollArea->setWidgetResizable(true);
        runLayout = new QWidget();
        runLayout->setObjectName(QStringLiteral("runLayout"));
        runLayout->setGeometry(QRect(0, 0, 307, 199));
        verticalLayout_17 = new QVBoxLayout(runLayout);
        verticalLayout_17->setSpacing(3);
        verticalLayout_17->setContentsMargins(11, 11, 11, 11);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        verticalLayout_17->setContentsMargins(3, 3, 3, 3);
        runTextEdit = new QPlainTextEdit(runLayout);
        runTextEdit->setObjectName(QStringLiteral("runTextEdit"));
        sizePolicy.setHeightForWidth(runTextEdit->sizePolicy().hasHeightForWidth());
        runTextEdit->setSizePolicy(sizePolicy);
        runTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        runTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_17->addWidget(runTextEdit);

        runScrollArea->setWidget(runLayout);

        verticalLayout_15->addWidget(runScrollArea);

        outputWidget->addTab(runTab, QString());

        verticalLayout_13->addWidget(outputWidget);

        statusWidget_2 = new QTabWidget(infoWidget);
        statusWidget_2->setObjectName(QStringLiteral("statusWidget_2"));
        sizePolicy.setHeightForWidth(statusWidget_2->sizePolicy().hasHeightForWidth());
        statusWidget_2->setSizePolicy(sizePolicy);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        statusWidget_2->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        statusWidget_2->addTab(tab_2, QString());

        verticalLayout_13->addWidget(statusWidget_2);


        controlLayout->addWidget(infoWidget);

        splitter->addWidget(otherWidget);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 25));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_File->menuAction());

        retranslateUi(MainWindow);

        outputWidget->setCurrentIndex(0);
        statusWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Micromouse Simulator", 0));
        runButton->setText(QApplication::translate("MainWindow", "Run", 0));
        buildProgressLabel->setText(QApplication::translate("MainWindow", "Build Progress", 0));
        buildButton->setText(QApplication::translate("MainWindow", "Build", 0));
        selectAlgorithmLabel->setText(QApplication::translate("MainWindow", "Select Algorithm", 0));
        buildAndRunButton->setText(QApplication::translate("MainWindow", "Build and Run", 0));
        outputWidget->setTabText(outputWidget->indexOf(buildTab), QApplication::translate("MainWindow", "Build Ouput", 0));
        outputWidget->setTabText(outputWidget->indexOf(runTab), QApplication::translate("MainWindow", "Run Ouput", 0));
        statusWidget_2->setTabText(statusWidget_2->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0));
        statusWidget_2->setTabText(statusWidget_2->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", 0));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
