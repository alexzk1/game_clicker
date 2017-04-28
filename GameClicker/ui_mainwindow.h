/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu 19. Jul 00:51:53 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qwindowtargeter.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionStart_Script;
    QAction *actionStop_Script;
    QAction *actionDummy;
    QAction *actionReload;
    QAction *actionCheck_Updates;
    QAction *actionSet_Script_s_Folder;
    QAction *actionWeb_Site;
    QAction *actionEnable_Remote;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listWidget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QWindowTargeter *wSelector;
    QGroupBox *groupBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QTableWidget *tableWidget;
    QPushButton *pushButton;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(617, 534);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/images/main.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionStart_Script = new QAction(MainWindow);
        actionStart_Script->setObjectName(QString::fromUtf8("actionStart_Script"));
        actionStart_Script->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/images/Launch.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart_Script->setIcon(icon1);
        actionStop_Script = new QAction(MainWindow);
        actionStop_Script->setObjectName(QString::fromUtf8("actionStop_Script"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Icons/images/Stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop_Script->setIcon(icon2);
        actionDummy = new QAction(MainWindow);
        actionDummy->setObjectName(QString::fromUtf8("actionDummy"));
        actionDummy->setEnabled(false);
        actionReload = new QAction(MainWindow);
        actionReload->setObjectName(QString::fromUtf8("actionReload"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Icons/images/Reload all.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReload->setIcon(icon3);
        actionCheck_Updates = new QAction(MainWindow);
        actionCheck_Updates->setObjectName(QString::fromUtf8("actionCheck_Updates"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Icons/images/update.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCheck_Updates->setIcon(icon4);
        actionSet_Script_s_Folder = new QAction(MainWindow);
        actionSet_Script_s_Folder->setObjectName(QString::fromUtf8("actionSet_Script_s_Folder"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Icons/images/Add folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSet_Script_s_Folder->setIcon(icon5);
        actionWeb_Site = new QAction(MainWindow);
        actionWeb_Site->setObjectName(QString::fromUtf8("actionWeb_Site"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Icons/images/Web.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWeb_Site->setIcon(icon6);
        actionEnable_Remote = new QAction(MainWindow);
        actionEnable_Remote->setObjectName(QString::fromUtf8("actionEnable_Remote"));
        actionEnable_Remote->setCheckable(true);
        actionEnable_Remote->setEnabled(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Icons/images/Network.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEnable_Remote->setIcon(icon7);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout_2->addWidget(listWidget);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        mainToolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::RightToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(140, 424));
        dockWidget->setMaximumSize(QSize(140, 524287));
        dockWidget->setFeatures(QDockWidget::DockWidgetMovable);
        dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        wSelector = new QWindowTargeter(dockWidgetContents);
        wSelector->setObjectName(QString::fromUtf8("wSelector"));
        wSelector->setMinimumSize(QSize(100, 220));
        wSelector->setMaximumSize(QSize(1000, 220));

        verticalLayout->addWidget(wSelector);

        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 40));
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(3, 16, 91, 20));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(98, 14, 20, 23));
        pushButton_2->setMouseTracking(true);
        pushButton_2->setAutoDefault(false);
        pushButton_2->setDefault(true);
        pushButton_2->setFlat(true);

        verticalLayout->addWidget(groupBox);

        tableWidget = new QTableWidget(dockWidgetContents);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setColumnCount(2);
        tableWidget->horizontalHeader()->setDefaultSectionSize(27);
        tableWidget->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableWidget);

        pushButton = new QPushButton(dockWidgetContents);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        mainToolBar->addAction(actionStart_Script);
        mainToolBar->addAction(actionStop_Script);
        mainToolBar->addAction(actionEnable_Remote);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionReload);
        mainToolBar->addAction(actionSet_Script_s_Folder);
        mainToolBar->addAction(actionWeb_Site);
        mainToolBar->addAction(actionCheck_Updates);
        mainToolBar->addSeparator();
        toolBar->addAction(actionDummy);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Game Clicker", 0));
        actionStart_Script->setText(QApplication::translate("MainWindow", "Start Script", 0));
#ifndef QT_NO_TOOLTIP
        actionStart_Script->setToolTip(QApplication::translate("MainWindow", "Starts or Re-starts script.", 0));
#endif // QT_NO_TOOLTIP
        actionStop_Script->setText(QApplication::translate("MainWindow", "Stop Script", 0));
#ifndef QT_NO_TOOLTIP
        actionStop_Script->setToolTip(QApplication::translate("MainWindow", "Immedial stop and unregister hotkeys.", 0));
#endif // QT_NO_TOOLTIP
        actionDummy->setText(QApplication::translate("MainWindow", "dummy", 0));
        actionReload->setText(QApplication::translate("MainWindow", "Reload List", 0));
#ifndef QT_NO_TOOLTIP
        actionReload->setToolTip(QApplication::translate("MainWindow", "Reload scripts' list", 0));
#endif // QT_NO_TOOLTIP
        actionCheck_Updates->setText(QApplication::translate("MainWindow", "Check Updates", 0));
        actionSet_Script_s_Folder->setText(QApplication::translate("MainWindow", "Set Script's Folder", 0));
#ifndef QT_NO_TOOLTIP
        actionSet_Script_s_Folder->setToolTip(QApplication::translate("MainWindow", "Adds user's script search path", 0));
#endif // QT_NO_TOOLTIP
        actionWeb_Site->setText(QApplication::translate("MainWindow", "Web Site", 0));
#ifndef QT_NO_TOOLTIP
        actionWeb_Site->setToolTip(QApplication::translate("MainWindow", "Go to Web site", 0));
#endif // QT_NO_TOOLTIP
        actionEnable_Remote->setText(QApplication::translate("MainWindow", "Enable Remote", 0));
#ifndef QT_NO_TOOLTIP
        actionEnable_Remote->setToolTip(QApplication::translate("MainWindow", "Allow control from remote", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        listWidget->setToolTip(QApplication::translate("MainWindow", "Select Script to start. Double click to edit it.", 0));
#endif // QT_NO_TOOLTIP
        dockWidget->setWindowTitle(QApplication::translate("MainWindow", "Windows List", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Add by Title", 0));
#ifndef QT_NO_TOOLTIP
        lineEdit->setToolTip(QApplication::translate("MainWindow", "Type window title to add to list.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushButton_2->setToolTip(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Click to add windows by typed title. Each window is added once.</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        pushButton_2->setText(QApplication::translate("MainWindow", "+1", 0));
#ifndef QT_NO_TOOLTIP
        tableWidget->setToolTip(QApplication::translate("MainWindow", "Windows list. Double click to bring to front one.", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pushButton->setToolTip(QApplication::translate("MainWindow", "Clear the windows' list.", 0));
#endif // QT_NO_TOOLTIP
        pushButton->setText(QApplication::translate("MainWindow", "Clear", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
