/********************************************************************************
** Form generated from reading UI file 'qwindowtargeter.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QWINDOWTARGETER_H
#define UI_QWINDOWTARGETER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QWindowTargeter
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGroupBox *groupBox;
    QLabel *lX;
    QLabel *lY;
    QToolButton *toolButton;
    QLabel *lC;

    void setupUi(QGroupBox *QWindowTargeter)
    {
        if (QWindowTargeter->objectName().isEmpty())
            QWindowTargeter->setObjectName(QStringLiteral("QWindowTargeter"));
        QWindowTargeter->resize(108, 218);
        verticalLayout = new QVBoxLayout(QWindowTargeter);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(QWindowTargeter);
        label->setObjectName(QStringLiteral("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/Icons/Target")));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        groupBox = new QGroupBox(QWindowTargeter);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        lX = new QLabel(groupBox);
        lX->setObjectName(QStringLiteral("lX"));
        lX->setGeometry(QRect(5, 7, 46, 13));
        lY = new QLabel(groupBox);
        lY->setObjectName(QStringLiteral("lY"));
        lY->setGeometry(QRect(5, 25, 46, 13));
        toolButton = new QToolButton(groupBox);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(7, 66, 76, 19));
        lC = new QLabel(groupBox);
        lC->setObjectName(QStringLiteral("lC"));
        lC->setGeometry(QRect(5, 43, 76, 16));

        verticalLayout->addWidget(groupBox);


        retranslateUi(QWindowTargeter);

        QMetaObject::connectSlotsByName(QWindowTargeter);
    } // setupUi

    void retranslateUi(QGroupBox *QWindowTargeter)
    {
        QWindowTargeter->setWindowTitle(QApplication::translate("QWindowTargeter", "Window Targeter", Q_NULLPTR));
        QWindowTargeter->setTitle(QApplication::translate("QWindowTargeter", "Select Window", Q_NULLPTR));
        label->setText(QString());
        groupBox->setTitle(QString());
        lX->setText(QApplication::translate("QWindowTargeter", "X:", Q_NULLPTR));
        lY->setText(QApplication::translate("QWindowTargeter", "Y:", Q_NULLPTR));
        toolButton->setText(QApplication::translate("QWindowTargeter", "Copy", Q_NULLPTR));
        lC->setText(QApplication::translate("QWindowTargeter", "C:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QWindowTargeter: public Ui_QWindowTargeter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QWINDOWTARGETER_H
