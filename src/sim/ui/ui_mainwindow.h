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
#include <QtWidgets/QSpinBox>
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
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *importMazesButton;
    QTableWidget *mazeFilesTable;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_10;
    QWidget *controlWidget_2;
    QGridLayout *gridLayout_6;
    QPushButton *buildMazeAlgoButton;
    QSpinBox *widthSpinBox;
    QSpinBox *heightSpinBox;
    QComboBox *selectMazeAlgorithmComboBox;
    QPushButton *editMazeAlgoButton;
    QPushButton *runMazeAlgoButton;
    QPushButton *importMazeAlgoButton;
    QWidget *infoWidget_3;
    QGridLayout *gridLayout_7;
    QSplitter *infoWidgetSplitter_2;
    QTabWidget *outputWidget_3;
    QWidget *buildTab_3;
    QVBoxLayout *verticalLayout_8;
    QPlainTextEdit *mazeBuildTextEdit;
    QWidget *runTab_;
    QVBoxLayout *verticalLayout_9;
    QPlainTextEdit *mazeRunTextEdit;
    QWidget *mouseTab;
    QVBoxLayout *controlLayout;
    QWidget *controlWidget;
    QGridLayout *gridLayout;
    QPushButton *buildButton;
    QPushButton *importMouseAlgoButton;
    QLabel *selectAlgorithmLabel;
    QPushButton *editParametersButton;
    QPushButton *pauseButton;
    QComboBox *selectAlgorithmComboBox;
    QPushButton *runButton;
    QSlider *horizontalSlider;
    QLabel *label;
    QPushButton *editMouseAlgoButton;
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
        MainWindow->resize(774, 480);
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
        widget = new QWidget(mazeTab);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        importMazesButton = new QPushButton(widget);
        importMazesButton->setObjectName(QStringLiteral("importMazesButton"));

        horizontalLayout->addWidget(importMazesButton);


        verticalLayout_6->addWidget(widget);

        mazeFilesTable = new QTableWidget(mazeTab);
        mazeFilesTable->setObjectName(QStringLiteral("mazeFilesTable"));

        verticalLayout_6->addWidget(mazeFilesTable);

        otherWidget->addTab(mazeTab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_10 = new QVBoxLayout(tab_3);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        controlWidget_2 = new QWidget(tab_3);
        controlWidget_2->setObjectName(QStringLiteral("controlWidget_2"));
        gridLayout_6 = new QGridLayout(controlWidget_2);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(3, -1, -1, 6);
        buildMazeAlgoButton = new QPushButton(controlWidget_2);
        buildMazeAlgoButton->setObjectName(QStringLiteral("buildMazeAlgoButton"));

        gridLayout_6->addWidget(buildMazeAlgoButton, 1, 1, 1, 1);

        widthSpinBox = new QSpinBox(controlWidget_2);
        widthSpinBox->setObjectName(QStringLiteral("widthSpinBox"));
        widthSpinBox->setMinimum(1);
        widthSpinBox->setValue(16);

        gridLayout_6->addWidget(widthSpinBox, 1, 5, 1, 1);

        heightSpinBox = new QSpinBox(controlWidget_2);
        heightSpinBox->setObjectName(QStringLiteral("heightSpinBox"));
        heightSpinBox->setMinimum(1);
        heightSpinBox->setValue(16);

        gridLayout_6->addWidget(heightSpinBox, 1, 6, 1, 1);

        selectMazeAlgorithmComboBox = new QComboBox(controlWidget_2);
        selectMazeAlgorithmComboBox->setObjectName(QStringLiteral("selectMazeAlgorithmComboBox"));
        sizePolicy.setHeightForWidth(selectMazeAlgorithmComboBox->sizePolicy().hasHeightForWidth());
        selectMazeAlgorithmComboBox->setSizePolicy(sizePolicy);

        gridLayout_6->addWidget(selectMazeAlgorithmComboBox, 0, 1, 1, 1);

        editMazeAlgoButton = new QPushButton(controlWidget_2);
        editMazeAlgoButton->setObjectName(QStringLiteral("editMazeAlgoButton"));

        gridLayout_6->addWidget(editMazeAlgoButton, 0, 3, 1, 1);

        runMazeAlgoButton = new QPushButton(controlWidget_2);
        runMazeAlgoButton->setObjectName(QStringLiteral("runMazeAlgoButton"));

        gridLayout_6->addWidget(runMazeAlgoButton, 1, 3, 1, 1);

        importMazeAlgoButton = new QPushButton(controlWidget_2);
        importMazeAlgoButton->setObjectName(QStringLiteral("importMazeAlgoButton"));

        gridLayout_6->addWidget(importMazeAlgoButton, 0, 5, 1, 2);


        verticalLayout_10->addWidget(controlWidget_2);

        infoWidget_3 = new QWidget(tab_3);
        infoWidget_3->setObjectName(QStringLiteral("infoWidget_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(infoWidget_3->sizePolicy().hasHeightForWidth());
        infoWidget_3->setSizePolicy(sizePolicy1);
        gridLayout_7 = new QGridLayout(infoWidget_3);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(3, -1, -1, -1);
        infoWidgetSplitter_2 = new QSplitter(infoWidget_3);
        infoWidgetSplitter_2->setObjectName(QStringLiteral("infoWidgetSplitter_2"));
        infoWidgetSplitter_2->setOrientation(Qt::Vertical);
        infoWidgetSplitter_2->setHandleWidth(9);
        infoWidgetSplitter_2->setChildrenCollapsible(false);
        outputWidget_3 = new QTabWidget(infoWidgetSplitter_2);
        outputWidget_3->setObjectName(QStringLiteral("outputWidget_3"));
        sizePolicy.setHeightForWidth(outputWidget_3->sizePolicy().hasHeightForWidth());
        outputWidget_3->setSizePolicy(sizePolicy);
        buildTab_3 = new QWidget();
        buildTab_3->setObjectName(QStringLiteral("buildTab_3"));
        verticalLayout_8 = new QVBoxLayout(buildTab_3);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(6, 6, 6, 6);
        mazeBuildTextEdit = new QPlainTextEdit(buildTab_3);
        mazeBuildTextEdit->setObjectName(QStringLiteral("mazeBuildTextEdit"));
        sizePolicy.setHeightForWidth(mazeBuildTextEdit->sizePolicy().hasHeightForWidth());
        mazeBuildTextEdit->setSizePolicy(sizePolicy);
        mazeBuildTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        mazeBuildTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_8->addWidget(mazeBuildTextEdit);

        outputWidget_3->addTab(buildTab_3, QString());
        runTab_ = new QWidget();
        runTab_->setObjectName(QStringLiteral("runTab_"));
        verticalLayout_9 = new QVBoxLayout(runTab_);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(6, 6, 6, 6);
        mazeRunTextEdit = new QPlainTextEdit(runTab_);
        mazeRunTextEdit->setObjectName(QStringLiteral("mazeRunTextEdit"));
        sizePolicy.setHeightForWidth(mazeRunTextEdit->sizePolicy().hasHeightForWidth());
        mazeRunTextEdit->setSizePolicy(sizePolicy);
        mazeRunTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        mazeRunTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_9->addWidget(mazeRunTextEdit);

        outputWidget_3->addTab(runTab_, QString());
        infoWidgetSplitter_2->addWidget(outputWidget_3);

        gridLayout_7->addWidget(infoWidgetSplitter_2, 0, 0, 1, 1);


        verticalLayout_10->addWidget(infoWidget_3);

        otherWidget->addTab(tab_3, QString());
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

        importMouseAlgoButton = new QPushButton(controlWidget);
        importMouseAlgoButton->setObjectName(QStringLiteral("importMouseAlgoButton"));

        gridLayout->addWidget(importMouseAlgoButton, 0, 5, 1, 1);

        selectAlgorithmLabel = new QLabel(controlWidget);
        selectAlgorithmLabel->setObjectName(QStringLiteral("selectAlgorithmLabel"));
        selectAlgorithmLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(selectAlgorithmLabel, 0, 0, 1, 1);

        editParametersButton = new QPushButton(controlWidget);
        editParametersButton->setObjectName(QStringLiteral("editParametersButton"));

        gridLayout->addWidget(editParametersButton, 1, 5, 1, 1);

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

        gridLayout->addWidget(horizontalSlider, 2, 5, 1, 1);

        label = new QLabel(controlWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 2, 3, 1, 1);

        editMouseAlgoButton = new QPushButton(controlWidget);
        editMouseAlgoButton->setObjectName(QStringLiteral("editMouseAlgoButton"));

        gridLayout->addWidget(editMouseAlgoButton, 0, 4, 1, 1);


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
        menuBar->setGeometry(QRect(0, 0, 774, 22));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_File->menuAction());

        retranslateUi(MainWindow);

        otherWidget->setCurrentIndex(1);
        outputWidget_3->setCurrentIndex(0);
        outputWidget->setCurrentIndex(0);
        statusWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Micromouse Simulator", Q_NULLPTR));
        importMazesButton->setText(QApplication::translate("MainWindow", "Import Mazes", Q_NULLPTR));
        otherWidget->setTabText(otherWidget->indexOf(mazeTab), QApplication::translate("MainWindow", "Maze Files", Q_NULLPTR));
        buildMazeAlgoButton->setText(QApplication::translate("MainWindow", "Build", Q_NULLPTR));
        editMazeAlgoButton->setText(QApplication::translate("MainWindow", "Edit", Q_NULLPTR));
        runMazeAlgoButton->setText(QApplication::translate("MainWindow", "Run", Q_NULLPTR));
        importMazeAlgoButton->setText(QApplication::translate("MainWindow", "Import", Q_NULLPTR));
        outputWidget_3->setTabText(outputWidget_3->indexOf(buildTab_3), QApplication::translate("MainWindow", "Build Ouput", Q_NULLPTR));
        outputWidget_3->setTabText(outputWidget_3->indexOf(runTab_), QApplication::translate("MainWindow", "Run Ouput", Q_NULLPTR));
        otherWidget->setTabText(otherWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Maze Algorithms", Q_NULLPTR));
        buildButton->setText(QApplication::translate("MainWindow", "Build", Q_NULLPTR));
        importMouseAlgoButton->setText(QApplication::translate("MainWindow", "Import", Q_NULLPTR));
        selectAlgorithmLabel->setText(QApplication::translate("MainWindow", "Algorithm", Q_NULLPTR));
        editParametersButton->setText(QApplication::translate("MainWindow", "Build and Run", Q_NULLPTR));
        pauseButton->setText(QApplication::translate("MainWindow", "Pause", Q_NULLPTR));
        runButton->setText(QApplication::translate("MainWindow", "Run", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Speed", Q_NULLPTR));
        editMouseAlgoButton->setText(QApplication::translate("MainWindow", "Edit", Q_NULLPTR));
        outputWidget->setTabText(outputWidget->indexOf(buildTab), QApplication::translate("MainWindow", "Build Ouput", Q_NULLPTR));
        outputWidget->setTabText(outputWidget->indexOf(runTab), QApplication::translate("MainWindow", "Run Ouput", Q_NULLPTR));
        statusWidget_2->setTabText(statusWidget_2->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", Q_NULLPTR));
        statusWidget_2->setTabText(statusWidget_2->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", Q_NULLPTR));
        otherWidget->setTabText(otherWidget->indexOf(mouseTab), QApplication::translate("MainWindow", "Mouse Algorithms", Q_NULLPTR));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
