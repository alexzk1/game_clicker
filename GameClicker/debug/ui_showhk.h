/********************************************************************************
** Form generated from reading UI file 'showhk.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWHK_H
#define UI_SHOWHK_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_ShowHK
{
public:
    QPushButton *pushButton;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *ShowHK)
    {
        if (ShowHK->objectName().isEmpty())
            ShowHK->setObjectName(QStringLiteral("ShowHK"));
        ShowHK->resize(415, 315);
        ShowHK->setMinimumSize(QSize(415, 315));
        ShowHK->setMaximumSize(QSize(415, 315));
        QIcon icon;
        icon.addFile(QStringLiteral(":/Icons/images/main.png"), QSize(), QIcon::Normal, QIcon::Off);
        ShowHK->setWindowIcon(icon);
        ShowHK->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        ShowHK->setModal(true);
        pushButton = new QPushButton(ShowHK);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 290, 111, 23));
        textBrowser = new QTextBrowser(ShowHK);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(0, 0, 415, 280));
        textBrowser->setOpenExternalLinks(true);

        retranslateUi(ShowHK);

        QMetaObject::connectSlotsByName(ShowHK);
    } // setupUi

    void retranslateUi(QDialog *ShowHK)
    {
        ShowHK->setWindowTitle(QApplication::translate("ShowHK", "Hot Keys List", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ShowHK", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ShowHK: public Ui_ShowHK {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWHK_H
