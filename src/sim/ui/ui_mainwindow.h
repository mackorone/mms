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
#include <QtWidgets/QPushButton>
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
    QPushButton *buildButton;
    QLabel *selectAlgorithmLabel;
    QPushButton *buildAndRunButton;
    QComboBox *selectAlgorithmComboBox;
    QWidget *infoWidget;
    QGridLayout *gridLayout_3;
    QSplitter *infoWidgetSplitter;
    QTabWidget *outputWidget;
    QWidget *buildTab;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *buildTextEdit;
    QWidget *runTab;
    QVBoxLayout *verticalLayout_2;
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
        sizePolicy.setHeightForWidth(mapWidget->sizePolicy().hasHeightForWidth());
        mapWidget->setSizePolicy(sizePolicy);
        mapWidget->setMinimumSize(QSize(0, 0));
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

        gridLayout->addWidget(runButton, 1, 1, 1, 1);

        buildButton = new QPushButton(controlWidget);
        buildButton->setObjectName(QStringLiteral("buildButton"));

        gridLayout->addWidget(buildButton, 1, 0, 1, 1);

        selectAlgorithmLabel = new QLabel(controlWidget);
        selectAlgorithmLabel->setObjectName(QStringLiteral("selectAlgorithmLabel"));
        selectAlgorithmLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(selectAlgorithmLabel, 0, 0, 1, 1);

        buildAndRunButton = new QPushButton(controlWidget);
        buildAndRunButton->setObjectName(QStringLiteral("buildAndRunButton"));

        gridLayout->addWidget(buildAndRunButton, 1, 2, 1, 1);

        selectAlgorithmComboBox = new QComboBox(controlWidget);
        selectAlgorithmComboBox->setObjectName(QStringLiteral("selectAlgorithmComboBox"));
        sizePolicy.setHeightForWidth(selectAlgorithmComboBox->sizePolicy().hasHeightForWidth());
        selectAlgorithmComboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(selectAlgorithmComboBox, 0, 1, 1, 2);


        controlLayout->addWidget(controlWidget);

        infoWidget = new QWidget(otherWidget);
        infoWidget->setObjectName(QStringLiteral("infoWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(infoWidget->sizePolicy().hasHeightForWidth());
        infoWidget->setSizePolicy(sizePolicy1);
        gridLayout_3 = new QGridLayout(infoWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(3, -1, -1, -1);
        infoWidgetSplitter = new QSplitter(infoWidget);
        infoWidgetSplitter->setObjectName(QStringLiteral("infoWidgetSplitter"));
        infoWidgetSplitter->setOrientation(Qt::Vertical);
        infoWidgetSplitter->setHandleWidth(9);
        infoWidgetSplitter->setChildrenCollapsible(false);
        outputWidget = new QTabWidget(infoWidgetSplitter);
        outputWidget->setObjectName(QStringLiteral("outputWidget"));
        sizePolicy.setHeightForWidth(outputWidget->sizePolicy().hasHeightForWidth());
        outputWidget->setSizePolicy(sizePolicy);
        buildTab = new QWidget();
        buildTab->setObjectName(QStringLiteral("buildTab"));
        verticalLayout = new QVBoxLayout(buildTab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        buildTextEdit = new QPlainTextEdit(buildTab);
        buildTextEdit->setObjectName(QStringLiteral("buildTextEdit"));
        sizePolicy.setHeightForWidth(buildTextEdit->sizePolicy().hasHeightForWidth());
        buildTextEdit->setSizePolicy(sizePolicy);
        buildTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        buildTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(buildTextEdit);

        outputWidget->addTab(buildTab, QString());
        runTab = new QWidget();
        runTab->setObjectName(QStringLiteral("runTab"));
        verticalLayout_2 = new QVBoxLayout(runTab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, 6, 6, 6);
        runTextEdit = new QPlainTextEdit(runTab);
        runTextEdit->setObjectName(QStringLiteral("runTextEdit"));
        sizePolicy.setHeightForWidth(runTextEdit->sizePolicy().hasHeightForWidth());
        runTextEdit->setSizePolicy(sizePolicy);
        runTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        runTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_2->addWidget(runTextEdit);

        outputWidget->addTab(runTab, QString());
        infoWidgetSplitter->addWidget(outputWidget);
        statusWidget_2 = new QTabWidget(infoWidgetSplitter);
        statusWidget_2->setObjectName(QStringLiteral("statusWidget_2"));
        sizePolicy.setHeightForWidth(statusWidget_2->sizePolicy().hasHeightForWidth());
        statusWidget_2->setSizePolicy(sizePolicy);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        statusWidget_2->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        statusWidget_2->addTab(tab_2, QString());
        infoWidgetSplitter->addWidget(statusWidget_2);

        gridLayout_3->addWidget(infoWidgetSplitter, 0, 0, 1, 1);


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

        outputWidget->setCurrentIndex(1);
        statusWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Micromouse Simulator", 0));
        runButton->setText(QApplication::translate("MainWindow", "Run", 0));
        buildButton->setText(QApplication::translate("MainWindow", "Build", 0));
        selectAlgorithmLabel->setText(QApplication::translate("MainWindow", "Algorithm", 0));
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
