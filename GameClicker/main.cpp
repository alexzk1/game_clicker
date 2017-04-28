#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QProcess::execute(QString("watcher.exe %1").arg(GetCurrentProcessId()));
    QTranslator translator(0);
    translator.load( QString("gc_") + QLocale::system ().name().left(2), "." );
    a.installTranslator( &translator );

    MainWindow w;
    w.show();
    return a.exec();
}
