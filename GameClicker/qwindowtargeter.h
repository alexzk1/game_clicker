#ifndef QWINDOWTARGETER_H
#define QWINDOWTARGETER_H

#include <QtWidgets/QGroupBox>
#include <stdint.h>

#include "fake_types.h"

#include <QMouseEvent>

namespace Ui {
    class QWindowTargeter;
}

class QWindowTargeter : public QGroupBox
{
    Q_OBJECT

public:
    explicit QWindowTargeter(QWidget *parent = 0);
    ~QWindowTargeter();
    int x(){return wX;}
    int y(){return wY;}

    static COLORREF getColor(HWND win, int x, int y);
private:
    Ui::QWindowTargeter *ui;
    bool eventFilter ( QObject * watched, QEvent * event );
    HWND getCoveredWindow(QMouseEvent *e);
    void calcCoords(QMouseEvent *e);
    void RestoreWin(HWND win);
    QPixmap Target;
    HWND old_win;
    HWND SelectedTarget;
    int wX, wY;
    QString wColor;
signals:
    void windowSelected(HWND win);
    void mainMinimize();
    void mainRestore();
private slots:
    void on_toolButton_clicked();
};

#endif // QWINDOWTARGETER_H
