#include "qautomessagebox.h"
#include <QAbstractButton>
#include <QTimerEvent>

QAutoMessageBox::QAutoMessageBox(QWidget *parent) :
    QMessageBox(parent)
{
    timeout = 0;
    autoClose = false;
    currentTime = 0;
    timerid=-1;
}

void QAutoMessageBox::showEvent ( QShowEvent * event ) {
    currentTime = 0;
    QAbstractButton *b = button(toClick);
    if (b && autoClose)
    {
        bText = b->text();
    }
    if (autoClose) {
    timerid = this->startTimer(1000);
    }
    QMessageBox::showEvent(event);
}

void QAutoMessageBox::timerEvent(QTimerEvent *event)
{
    if (autoClose && event->timerId() == timerid)
    {
       currentTime++;
       QAbstractButton *b = button(toClick);
       if (b)
       {
           b->setText(QString("%1 (%2)").arg(bText).arg(timeout-currentTime));
       }
       if (currentTime>=timeout) {
          this->done(toClick);
       }
    }
    QMessageBox::timerEvent(event);
}

void QAutoMessageBox::hideEvent ( QHideEvent * event )
{
    QAbstractButton *b = button(toClick);
    if (b && autoClose)
    {
        b->setText(bText);
    }
    if (autoClose)
        this->killTimer(timerid);
    QMessageBox::hideEvent(event);
}
