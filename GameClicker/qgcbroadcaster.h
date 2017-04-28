#ifndef QGCBROADCASTER_H
#define QGCBROADCASTER_H

#include <QObject>
#include <QtNetwork>

class QGcBroadcaster : public QObject
{
    Q_OBJECT
public:
    explicit QGcBroadcaster(QObject *parent = 0);    
    bool signalBroadcast(QString signal);
signals:
    
public slots:
    
};

#endif // QGCBROADCASTER_H
