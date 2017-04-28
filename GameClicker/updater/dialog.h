#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDomDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>


namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void xmlFinished ( QNetworkReply * reply );
    void delayedEnableUI();
    void on_pushButton_2_clicked();
    void meter(qint64 value,qint64 max);
    void downloadFinished();

private:
    Ui::Dialog *ui;
    QDomDocument Doc;
    QNetworkAccessManager *xmlnet;
    QNetworkAccessManager *downnet;
    QNetworkReply* download;

    //bool SelfUpdate;

    void enableUI(bool enable);
    void processXml();
    bool checkFile(QString file,QString md5);
    bool showOkCancel(QString txt,QString detailed);
    void startFile();
    void doneFiles();
};

#endif // DIALOG_H
