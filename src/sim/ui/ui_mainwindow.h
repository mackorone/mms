/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
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
    QTabWidget *otherWidget;
    QWidget *mazeTab;
    QVBoxLayout *verticalLayout_6;
    QTabWidget *tabWidget;
    QWidget *mazeFilesTab;
    QVBoxLayout *verticalLayout_7;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *importMazesButton;
    QTableWidget *mazeFilesTable;
    QWidget *mazeAlgorithmsTab;
    QVBoxLayout *verticalLayout_5;
    QWidget *controlWidget_2;
    QGridLayout *gridLayout_4;
    QPushButton *mazeImportButton;
    QPushButton *mazeBuildButton;
    QPushButton *mazeRunButton;
    QPushButton *buildAndRunButton_2;
    QLabel *selectAlgorithmLabel_2;
    QComboBox *selectMazeAlgorithmComboBox;
    QWidget *infoWidget_2;
    QGridLayout *gridLayout_5;
    QSplitter *infoWidgetSplitter_2;
    QTabWidget *outputWidget_2;
    QWidget *buildTab_2;
    QVBoxLayout *verticalLayout_3;
    QPlainTextEdit *mazeBuildTextEdit;
    QWidget *runTab_2;
    QVBoxLayout *verticalLayout_4;
    QPlainTextEdit *mazeRunTextEdit;
    QWidget *mouseTab;
    QVBoxLayout *controlLayout;
    QWidget *controlWidget;
    QGridLayout *gridLayout;
    QPushButton *buildButton;
    QPushButton *importButton;
    QLabel *selectAlgorithmLabel;
    QPushButton *buildAndRunButton;
    QPushButton *pauseButton;
    QComboBox *selectAlgorithmComboBox;
    QPushButton *runButton;
    QSlider *horizontalSlider;
    QLabel *label;
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
        MainWindow->resize(663, 480);
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
        gridLayout_2->setHorizontalSpacing(6);
        gridLayout_2->setContentsMargins(9, 9, 9, 9);
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
        mapWidget->setMinimumSize(QSize(300, 0));
        mapLayout = new QVBoxLayout(mapWidget);
        mapLayout->setSpacing(6);
        mapLayout->setContentsMargins(11, 11, 11, 11);
        mapLayout->setObjectName(QStringLiteral("mapLayout"));
        mapLayout->setContentsMargins(-1, -1, 3, -1);
        splitter->addWidget(mapWidget);
        otherWidget = new QTabWidget(splitter);
        otherWidget->setObjectName(QStringLiteral("otherWidget"));
        otherWidget->setStyleSheet(QStringLiteral(""));
        mazeTab = new QWidget();
        mazeTab->setObjectName(QStringLiteral("mazeTab"));
        verticalLayout_6 = new QVBoxLayout(mazeTab);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        tabWidget = new QTabWidget(mazeTab);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        mazeFilesTab = new QWidget();
        mazeFilesTab->setObjectName(QStringLiteral("mazeFilesTab"));
        verticalLayout_7 = new QVBoxLayout(mazeFilesTab);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        widget = new QWidget(mazeFilesTab);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        importMazesButton = new QPushButton(widget);
        importMazesButton->setObjectName(QStringLiteral("importMazesButton"));

        horizontalLayout->addWidget(importMazesButton);


        verticalLayout_7->addWidget(widget);

        mazeFilesTable = new QTableWidget(mazeFilesTab);
        mazeFilesTable->setObjectName(QStringLiteral("mazeFilesTable"));

        verticalLayout_7->addWidget(mazeFilesTable);

        tabWidget->addTab(mazeFilesTab, QString());
        mazeAlgorithmsTab = new QWidget();
        mazeAlgorithmsTab->setObjectName(QStringLiteral("mazeAlgorithmsTab"));
        verticalLayout_5 = new QVBoxLayout(mazeAlgorithmsTab);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        controlWidget_2 = new QWidget(mazeAlgorithmsTab);
        controlWidget_2->setObjectName(QStringLiteral("controlWidget_2"));
        gridLayout_4 = new QGridLayout(controlWidget_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(3, -1, -1, 6);
        mazeImportButton = new QPushButton(controlWidget_2);
        mazeImportButton->setObjectName(QStringLiteral("mazeImportButton"));

        gridLayout_4->addWidget(mazeImportButton, 0, 4, 1, 1);

        mazeBuildButton = new QPushButton(controlWidget_2);
        mazeBuildButton->setObjectName(QStringLiteral("mazeBuildButton"));

        gridLayout_4->addWidget(mazeBuildButton, 1, 0, 1, 1);

        mazeRunButton = new QPushButton(controlWidget_2);
        mazeRunButton->setObjectName(QStringLiteral("mazeRunButton"));

        gridLayout_4->addWidget(mazeRunButton, 1, 1, 1, 2);

        buildAndRunButton_2 = new QPushButton(controlWidget_2);
        buildAndRunButton_2->setObjectName(QStringLiteral("buildAndRunButton_2"));

        gridLayout_4->addWidget(buildAndRunButton_2, 1, 4, 1, 1);

        selectAlgorithmLabel_2 = new QLabel(controlWidget_2);
        selectAlgorithmLabel_2->setObjectName(QStringLiteral("selectAlgorithmLabel_2"));
        selectAlgorithmLabel_2->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(selectAlgorithmLabel_2, 0, 0, 1, 1);

        selectMazeAlgorithmComboBox = new QComboBox(controlWidget_2);
        selectMazeAlgorithmComboBox->setObjectName(QStringLiteral("selectMazeAlgorithmComboBox"));
        sizePolicy.setHeightForWidth(selectMazeAlgorithmComboBox->sizePolicy().hasHeightForWidth());
        selectMazeAlgorithmComboBox->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(selectMazeAlgorithmComboBox, 0, 1, 1, 2);


        verticalLayout_5->addWidget(controlWidget_2);

        infoWidget_2 = new QWidget(mazeAlgorithmsTab);
        infoWidget_2->setObjectName(QStringLiteral("infoWidget_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(infoWidget_2->sizePolicy().hasHeightForWidth());
        infoWidget_2->setSizePolicy(sizePolicy1);
        gridLayout_5 = new QGridLayout(infoWidget_2);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(3, -1, -1, -1);
        infoWidgetSplitter_2 = new QSplitter(infoWidget_2);
        infoWidgetSplitter_2->setObjectName(QStringLiteral("infoWidgetSplitter_2"));
        infoWidgetSplitter_2->setOrientation(Qt::Vertical);
        infoWidgetSplitter_2->setHandleWidth(9);
        infoWidgetSplitter_2->setChildrenCollapsible(false);
        outputWidget_2 = new QTabWidget(infoWidgetSplitter_2);
        outputWidget_2->setObjectName(QStringLiteral("outputWidget_2"));
        sizePolicy.setHeightForWidth(outputWidget_2->sizePolicy().hasHeightForWidth());
        outputWidget_2->setSizePolicy(sizePolicy);
        buildTab_2 = new QWidget();
        buildTab_2->setObjectName(QStringLiteral("buildTab_2"));
        verticalLayout_3 = new QVBoxLayout(buildTab_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(6, 6, 6, 6);
        mazeBuildTextEdit = new QPlainTextEdit(buildTab_2);
        mazeBuildTextEdit->setObjectName(QStringLiteral("mazeBuildTextEdit"));
        sizePolicy.setHeightForWidth(mazeBuildTextEdit->sizePolicy().hasHeightForWidth());
        mazeBuildTextEdit->setSizePolicy(sizePolicy);
        mazeBuildTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        mazeBuildTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_3->addWidget(mazeBuildTextEdit);

        outputWidget_2->addTab(buildTab_2, QString());
        runTab_2 = new QWidget();
        runTab_2->setObjectName(QStringLiteral("runTab_2"));
        verticalLayout_4 = new QVBoxLayout(runTab_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(6, 6, 6, 6);
        mazeRunTextEdit = new QPlainTextEdit(runTab_2);
        mazeRunTextEdit->setObjectName(QStringLiteral("mazeRunTextEdit"));
        sizePolicy.setHeightForWidth(mazeRunTextEdit->sizePolicy().hasHeightForWidth());
        mazeRunTextEdit->setSizePolicy(sizePolicy);
        mazeRunTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        mazeRunTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_4->addWidget(mazeRunTextEdit);

        outputWidget_2->addTab(runTab_2, QString());
        infoWidgetSplitter_2->addWidget(outputWidget_2);

        gridLayout_5->addWidget(infoWidgetSplitter_2, 0, 0, 1, 1);


        verticalLayout_5->addWidget(infoWidget_2);

        tabWidget->addTab(mazeAlgorithmsTab, QString());
        infoWidget_2->raise();
        controlWidget_2->raise();

        verticalLayout_6->addWidget(tabWidget);

        otherWidget->addTab(mazeTab, QString());
        mouseTab = new QWidget();
        mouseTab->setObjectName(QStringLiteral("mouseTab"));
        controlLayout = new QVBoxLayout(mouseTab);
        controlLayout->setSpacing(6);
        controlLayout->setContentsMargins(11, 11, 11, 11);
        controlLayout->setObjectName(QStringLiteral("controlLayout"));
        controlLayout->setContentsMargins(9, 9, 9, 9);
        controlWidget = new QWidget(mouseTab);
        controlWidget->setObjectName(QStringLiteral("controlWidget"));
        gridLayout = new QGridLayout(controlWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(3, -1, -1, 6);
        buildButton = new QPushButton(controlWidget);
        buildButton->setObjectName(QStringLiteral("buildButton"));

        gridLayout->addWidget(buildButton, 1, 0, 1, 1);

        importButton = new QPushButton(controlWidget);
        importButton->setObjectName(QStringLiteral("importButton"));

        gridLayout->addWidget(importButton, 0, 4, 1, 1);

        selectAlgorithmLabel = new QLabel(controlWidget);
        selectAlgorithmLabel->setObjectName(QStringLiteral("selectAlgorithmLabel"));
        selectAlgorithmLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(selectAlgorithmLabel, 0, 0, 1, 1);

        buildAndRunButton = new QPushButton(controlWidget);
        buildAndRunButton->setObjectName(QStringLiteral("buildAndRunButton"));

        gridLayout->addWidget(buildAndRunButton, 1, 4, 1, 1);

        pauseButton = new QPushButton(controlWidget);
        pauseButton->setObjectName(QStringLiteral("pauseButton"));

        gridLayout->addWidget(pauseButton, 2, 0, 1, 1);

        selectAlgorithmComboBox = new QComboBox(controlWidget);
        selectAlgorithmComboBox->setObjectName(QStringLiteral("selectAlgorithmComboBox"));
        sizePolicy.setHeightForWidth(selectAlgorithmComboBox->sizePolicy().hasHeightForWidth());
        selectAlgorithmComboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(selectAlgorithmComboBox, 0, 3, 1, 1);

        runButton = new QPushButton(controlWidget);
        runButton->setObjectName(QStringLiteral("runButton"));

        gridLayout->addWidget(runButton, 1, 3, 1, 1);

        horizontalSlider = new QSlider(controlWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 2, 4, 1, 1);

        label = new QLabel(controlWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 2, 3, 1, 1);


        controlLayout->addWidget(controlWidget);

        infoWidget = new QWidget(mouseTab);
        infoWidget->setObjectName(QStringLiteral("infoWidget"));
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

        otherWidget->addTab(mouseTab, QString());
        splitter->addWidget(otherWidget);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 663, 22));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_File->menuAction());

        retranslateUi(MainWindow);

        otherWidget->setCurrentIndex(0);
        tabWidget->setCurrentIndex(1);
        outputWidget_2->setCurrentIndex(1);
        outputWidget->setCurrentIndex(0);
        statusWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Micromouse Simulator", Q_NULLPTR));
        importMazesButton->setText(QApplication::translate("MainWindow", "Import Mazes", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(mazeFilesTab), QApplication::translate("MainWindow", "Maze Files", Q_NULLPTR));
        mazeImportButton->setText(QApplication::translate("MainWindow", "Import", Q_NULLPTR));
        mazeBuildButton->setText(QApplication::translate("MainWindow", "Build", Q_NULLPTR));
        mazeRunButton->setText(QApplication::translate("MainWindow", "Run", Q_NULLPTR));
        buildAndRunButton_2->setText(QApplication::translate("MainWindow", "Build and Run", Q_NULLPTR));
        selectAlgorithmLabel_2->setText(QApplication::translate("MainWindow", "Algorithm", Q_NULLPTR));
        outputWidget_2->setTabText(outputWidget_2->indexOf(buildTab_2), QApplication::translate("MainWindow", "Build Ouput", Q_NULLPTR));
        outputWidget_2->setTabText(outputWidget_2->indexOf(runTab_2), QApplication::translate("MainWindow", "Run Ouput", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(mazeAlgorithmsTab), QApplication::translate("MainWindow", "Maze Algorithms", Q_NULLPTR));
        otherWidget->setTabText(otherWidget->indexOf(mazeTab), QApplication::translate("MainWindow", "Maze", Q_NULLPTR));
        buildButton->setText(QApplication::translate("MainWindow", "Build", Q_NULLPTR));
        importButton->setText(QApplication::translate("MainWindow", "Import", Q_NULLPTR));
        selectAlgorithmLabel->setText(QApplication::translate("MainWindow", "Algorithm", Q_NULLPTR));
        buildAndRunButton->setText(QApplication::translate("MainWindow", "Build and Run", Q_NULLPTR));
        pauseButton->setText(QApplication::translate("MainWindow", "Pause", Q_NULLPTR));
        runButton->setText(QApplication::translate("MainWindow", "Run", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Speed", Q_NULLPTR));
        outputWidget->setTabText(outputWidget->indexOf(buildTab), QApplication::translate("MainWindow", "Build Ouput", Q_NULLPTR));
        outputWidget->setTabText(outputWidget->indexOf(runTab), QApplication::translate("MainWindow", "Run Ouput", Q_NULLPTR));
        statusWidget_2->setTabText(statusWidget_2->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", Q_NULLPTR));
        statusWidget_2->setTabText(statusWidget_2->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", Q_NULLPTR));
        otherWidget->setTabText(otherWidget->indexOf(mouseTab), QApplication::translate("MainWindow", "Mouse", Q_NULLPTR));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
