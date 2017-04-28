#include "dialog.h"
#include "ui_dialog.h"
#include <QBuffer>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include "../Common/qautomessagebox.h"
#include "../Common/md5.h"
#include <QProcess>

const int delayEnable = 3000;
const QString userAgent = "GameClicker Updater v1.0";



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->label_2->setVisible(false);
    ui->pushButton_2->setVisible(false);
    xmlnet   = NULL;
    downnet  = NULL;
    download = NULL;

    //SelfUpdate = false;
    on_pushButton_clicked();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::enableUI(bool enable)
{
    ui->pushButton->setEnabled(enable);
    ui->pushButton_2->setEnabled(enable);
}

void Dialog::delayedEnableUI()
{
    //protection against server overload by fast-clicking user
    enableUI(true);
}

void Dialog::on_pushButton_clicked()
{
    //check updates
    if (!xmlnet)
        xmlnet = new QNetworkAccessManager();
    enableUI(false);
    QNetworkRequest req;
    req.setUrl(QUrl("http://igra-em.ru/check_upd.php"));
    req.setRawHeader("User-Agent", userAgent.toAscii().data());

    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute,QNetworkRequest::AlwaysNetwork);
    req.setAttribute(QNetworkRequest::CacheSaveControlAttribute,false);

    connect(xmlnet, SIGNAL(finished(QNetworkReply*)),this, SLOT(xmlFinished(QNetworkReply*)));
    xmlnet->get(req);
}

void Dialog::xmlFinished (QNetworkReply* reply)
{
  if (reply)
  {
    reply->seek(0);
    if (reply->error()==0)
    {
        QBuffer buf;
        buf.open(QBuffer::ReadWrite);
        buf.write(reply->readAll());
        buf.seek(0);

#ifdef _DEBUG_
        QFile dmp("./upd_debug.xml");
        dmp.open(QFile::WriteOnly);
        dmp.write(buf.readAll());
        dmp.close();
        buf.seek(0);
#endif

        Doc.clear();
        Doc.setContent(&buf);
        processXml();

    }
    else
    {
        QMessageBox::critical(this,windowTitle(),QString(tr("Update Error. Network error with code %1")).arg(reply->error()));
    }
    reply->deleteLater();
  }
  else
  {
      QMessageBox::critical(this,windowTitle(),"NULL-reply. You should not see this error.");
  }
  if (xmlnet)
  {
      xmlnet->deleteLater();
      xmlnet = NULL;
  }
  QTimer::singleShot(delayEnable,this,SLOT(delayedEnableUI()));
}

bool Dialog::checkFile(QString file, QString md5)
{
    QString fn = QApplication::applicationDirPath()+"/"+file;
    MD5 md;
    FILE *fp = fopen(fn.toLocal8Bit().data(),"rb");
    if (fp)
    {
       md.update(fp);
       fclose(fp);
       md.finalize();
       char* h = md.hex_digest();
       QString hd = h;
       delete []h;
       return hd.toLower() == md5.toLower();
    }
    return false;
}

void Dialog::processXml()
{
    QDomElement docElem = Doc.documentElement();
    ui->listWidget->clear();
    if (!docElem.isNull())
    {
        QDomNodeList lst = Doc.elementsByTagName("file");int ind=0;
        for(QDomElement file = lst.at(ind).toElement();!file.isNull();
                        file = lst.at(++ind).toElement())
        {
            QDomElement md5  = file.firstChildElement("md5");
            if (checkFile(file.attribute("name"),md5.text()))
            {
                docElem.removeChild(lst.at(ind--));
                continue;
            }
            ulong       size = file.firstChildElement("size").text().toULong(0,0);
            QString isize ="";
            if (size<1024*1024) isize = QString("%2 (~%L1 KB)").arg(size/1024.,0,'f',2).arg(file.attribute("name"));
            else                isize = QString("%2 (~%L1 MB)").arg(size/(1024*1024.),0,'f',2).arg(file.attribute("name"));
            ui->listWidget->addItem(isize);
        }
#ifdef _DEBUG_
        qDebug() << Doc.toString();
#endif
    }
    if (Doc.elementsByTagName("file").count()==0)
    {
        ui->listWidget->addItem(tr("All files are current."));
        if (QApplication::arguments().count()>1 && QApplication::arguments().at(1) == "/autocheck")
            accept();
    }
    ui->pushButton_2->setVisible(Doc.elementsByTagName("file").count()>0);
}

void Dialog::on_pushButton_2_clicked()
{
    //download and install clicked
    enableUI(false);
    startFile();
}

void Dialog::startFile()
{
    QDomElement docElem = Doc.documentElement();
    if (!docElem.isNull())
    {
        QDomNodeList lst = Doc.elementsByTagName("file");
        if (lst.count())
        {
            QDomElement file = lst.item(0).toElement();
            QDomElement link = file.firstChildElement("link");
            if (!link.isNull())
            {
               downnet = new QNetworkAccessManager();
               QNetworkRequest req;
               req.setUrl(QUrl(link.text()));
               req.setRawHeader("User-Agent", userAgent.toAscii().data());
               req.setAttribute(QNetworkRequest::CacheLoadControlAttribute,QNetworkRequest::AlwaysNetwork);
               req.setAttribute(QNetworkRequest::CacheSaveControlAttribute,false);
               download = downnet->get(req);
               connect(download, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(meter(qint64,qint64)));
               connect(download, SIGNAL(finished()),this, SLOT(downloadFinished()));
               ui->progressBar->setVisible(true);
               ui->label_2->setVisible(true);
               ui->label_2->setText(file.attribute("name"));
            }
            else
               doneFiles();

        }
        else
            doneFiles();
    }
    else
        doneFiles();

}

void Dialog::doneFiles()
{
    ui->progressBar->setVisible(false);
    ui->label_2->setVisible(false);
    QTimer::singleShot(delayEnable,this,SLOT(delayedEnableUI()));
    QTimer::singleShot(delayEnable+1000,this,SLOT(on_pushButton_clicked()));
}

void Dialog::meter(qint64 value,qint64 max)
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(value);
}

bool Dialog::showOkCancel(QString txt,QString detailed)
{
    QAutoMessageBox msg(this);
    msg.setText(txt);
    msg.setWindowTitle(windowTitle());
    msg.setAutoClose(true);
    msg.setTimeout(30);
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setIcon(QMessageBox::Critical);
    if (detailed!="")
        msg.setDetailedText(detailed);

    return QMessageBox::Ok == msg.exec();
}

/*
  Updater self-replace should be triggered from server by special xml element.
  It should be exe and have attribute "run". If so - updater will launch it and close itself.

*/
void Dialog::downloadFinished()
{
    bool gonext = true;
    QDomElement file = Doc.elementsByTagName("file").item(0).toElement();
    QString msgFile = "";
    if (download->error()==0)
    {
        QBuffer buf;
        buf.open(QBuffer::ReadWrite);
        buf.write(download->readAll());
        buf.seek(0);
        msgFile=QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg(file.attribute("name"));
        bool goon = true;
        while (goon)
        {
          QFile dmp(msgFile);
          dmp.open(QFile::WriteOnly);
          dmp.write(buf.readAll());
          if (dmp.error())
          {
              goon = showOkCancel(tr("Cannot replace file. Close all running instances of the program(s) and press Ok, or Cancel to skip."),
                                  QString("File: %1\nError: %2").arg(msgFile).arg(dmp.errorString()));
          }
          else
              goon = false;
          dmp.close();
        }
        Doc.documentElement().removeChild(Doc.elementsByTagName("file").at(0));
    } else
    {
        gonext = showOkCancel(QString(tr("Download error with code %1. Press OK to retry.")).arg(download->error()),"");
        msgFile = "";
    }
    download->deleteLater();
    downnet->deleteLater();
    download = NULL;
    downnet  = NULL;
    if (file.hasAttribute("run") && msgFile!="")
    {
        if (!QProcess::startDetached(msgFile))
        {
            QMessageBox::critical(this,windowTitle(),
              QString(tr("Cannot launch self-updater. Please start in manually after this window will be closed. File is \"%1\"")).arg(msgFile));
        }
        reject();
    }
    if (gonext)
        startFile();
    else
        doneFiles();
}
