#include "qwindowtargeter.h"
#include "ui_qwindowtargeter.h"
#include <QMouseEvent>
#include <QDebug>
#include <QClipboard>

QWindowTargeter::QWindowTargeter(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::QWindowTargeter)
{
    ui->setupUi(this);
    ui->label->installEventFilter(this);
    Target.load(":/Icons/Target");
    ui->label->setPixmap(Target);
    old_win = NULL;
    SelectedTarget = NULL;
    wX=wY=0;
    wColor = CLR_INVALID;
}

QWindowTargeter::~QWindowTargeter()
{
    delete ui;
}

HWND QWindowTargeter::getCoveredWindow(QMouseEvent *e)
{
    //QPoint gpos = QCursor::pos();
    POINT p;
    //p.x = e->globalX();p.y=e->globalY();
    p.x = QCursor::pos().x();//e->globalX();
    p.y = QCursor::pos().y();//e->globalY();

    return WindowFromPoint(p);
}

COLORREF QWindowTargeter::getColor(HWND win, int x, int y)
{
    RECT wr;
    GetWindowRect(win,&wr);

    COLORREF r = CLR_INVALID;
    HDC dc = CreateDCW(L"DISPLAY",NULL,NULL,NULL);
    if (dc)
    {
        r = GetPixel(dc,x+wr.left,y+wr.top);
        DeleteDC(dc);
    }
    return r;
}

void QWindowTargeter::RestoreWin(HWND win)
{
    if (win)
    {
        InvalidateRect (old_win, NULL, TRUE);
        UpdateWindow (old_win);
        RedrawWindow (old_win, NULL, 0,RDW_FRAME+RDW_INVALIDATE + RDW_UPDATENOW + RDW_ALLCHILDREN);
    }
}

void QWindowTargeter::calcCoords(QMouseEvent *e)
{
    RECT wr;
    POINT p;

    p.x = QCursor::pos().x();//e->globalX();
    p.y = QCursor::pos().y();//e->globalY();
    HWND win = WindowFromPoint(p);
    GetWindowRect(win,&wr);
    wX = p.x - wr.left;
    wY = p.y - wr.top;
    ui->lX->setText(QString("X: %1").arg(wX));
    ui->lY->setText(QString("Y: %1").arg(wY));
    DWORD clr = getColor(win,wX,wY);
    QString nc = QString("%1%2%3").arg(GetRValue(clr),2,16,QChar('0')).arg(GetGValue(clr),2,16,QChar('0')).arg(GetBValue(clr),2,16,QChar('0'));

    wColor = QString("0x00%1").arg(nc.toUpper());
    ui->lC->setText(QString("<font color='#%2'>C: %1</font>").arg(wColor).arg(nc));
   // ui->lC->setAutoFillBackground(true);
   // ui->lC->setStyleSheet(QString("QLabel {background-color: #%1;}").arg(nc.toUpper()));
}

bool QWindowTargeter::eventFilter ( QObject * watched, QEvent * event )
{
    if (watched == ui->label)
    {
             if (event->type() == QEvent::MouseButtonPress) {
                 ui->label->setPixmap(QPixmap());
                 SetCursor(QCursor(QPixmap(":/Icons/Snipper")).handle());
                 mainMinimize();
                 return true;
             }
             else if (event->type() == QEvent::MouseButtonRelease) {
                 QMouseEvent *mEvent = (QMouseEvent*)event;
                 ui->label->setPixmap(Target);
                 RestoreWin(old_win);
                 old_win = NULL;
                 SelectedTarget = getCoveredWindow(mEvent);
                 calcCoords(mEvent);
                 mainRestore();
                 windowSelected(SelectedTarget);
                 return true;
             }
             else if (event->type() == QEvent::MouseMove)
             {
                 QMouseEvent *mEvent = (QMouseEvent*)event;
                 HWND win = getCoveredWindow(mEvent);
                 if (old_win!=win)
                     RestoreWin(old_win);
                 old_win = win;
                 RECT r;
                 GetWindowRect(win,&r);
                 HDC d=GetWindowDC(win);
                 if (d)
                 {
                     HPEN pen = CreatePen(PS_DASHDOT,2,RGB(0xFF,0,0));
                     HGDIOBJ hPrevPen = SelectObject (d, pen);
                     HGDIOBJ hPrevBrush = SelectObject (d, GetStockObject(HOLLOW_BRUSH));
                     Rectangle (d, 0, 0, r.right - r.left, r.bottom - r.top);
                     SelectObject (d, hPrevPen);
                     SelectObject (d, hPrevBrush);
                     // Finally release the DC.
                     ReleaseDC (win,d);
                     DeleteObject(pen);
                 }

                 return true;
             }
             else {
                 return false;
             }
         } else {
             // pass the event on to the parent class
             return QGroupBox::eventFilter(watched, event);
    }
}

void QWindowTargeter::on_toolButton_clicked()
{
    //copy coords
    QApplication::clipboard()->setText(QString("(%1,%2);color = %3").arg(wX).arg(wY).arg(wColor));
}
