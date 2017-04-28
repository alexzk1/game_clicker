#ifndef QAUTOMESSAGEBOX_H
#define QAUTOMESSAGEBOX_H

#include <QMessageBox>

class QAutoMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit QAutoMessageBox(QWidget *parent = 0);

    void setAutoClose(bool enabled, QMessageBox::StandardButton btn = QMessageBox::Ok)
                     {autoClose = enabled;toClick = btn;}
    void setTimeout(int seconds)
                     {timeout = seconds;}
signals:

public slots:
private:
    int  timeout;
    bool autoClose;
    int  currentTime;
    QMessageBox::StandardButton toClick;
    QString bText;
    int timerid;
 protected:
    virtual void showEvent ( QShowEvent * event );
    virtual void timerEvent(QTimerEvent *event);
    virtual void hideEvent ( QHideEvent * event );
};

#endif // QAUTOMESSAGEBOX_H
