#include <QtGui/QApplication>
#include "dialog.h"
#include <QWindowsVistaStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new QWindowsVistaStyle);
    Dialog w;
    w.show();

    return a.exec();
}
