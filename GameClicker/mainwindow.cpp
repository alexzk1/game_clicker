#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qwindowtargeter.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QDesktopServices>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QToolTip>
#include <showhk.h>
#include <Versions.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    Script(this,this),
    Settings(QCoreApplication::applicationDirPath()+"/gcsettings.ini",QSettings::IniFormat)
{

    ui->setupUi(this);
    on_pushButton_clicked();
    //lState = new QLabel(ui->mainToolBar);

    //lState->setGeometry();
    done_forhk();
    connect(ui->wSelector,SIGNAL(windowSelected(HWND)),this,SLOT(windowAdd(HWND)));
    connect(ui->wSelector,SIGNAL(mainMinimize()),this,SLOT(showMinimized()));
    connect(ui->wSelector,SIGNAL(mainRestore()),this,SLOT(bringBack()));
    connect(&Script,SIGNAL(started()),this,SLOT(wait_forhk()));
    connect(&Script,SIGNAL(stopped()),this,SLOT(done_forhk()));
    on_actionReload_triggered();

    loadSettings();
    setupButtons();

    QLabel *apiv = new QLabel;
    apiv->setText(QString("Exe Ver: %1; API Ver: %2; (c) 2011-2012 Alexey Z.").arg(CLICKER_VER).arg(GCAPI_VER));
    ui->statusBar->addPermanentWidget(apiv);

}

MainWindow::~MainWindow()
{
    Settings.setValue("MainWindow",saveState(5));
    Settings.setValue("Remote",ui->actionEnable_Remote->isChecked());
    Settings.setValue("SrchFor",ui->lineEdit->text());

    disconnect(&Script,SIGNAL(started()),this,SLOT(wait_forhk()));
    disconnect(&Script,SIGNAL(stopped()),this,SLOT(done_forhk()));
    unsetHKAll();
    delete ui;
}

void MainWindow::setupButtons()
{
  //i want buttons on toolbar be same sized
   int hm = 0;
   QObject *chld;
   foreach(chld,ui->mainToolBar->children())
   {
       if (!strcmp(chld->metaObject()->className(),"QToolButton"))
       {
           QToolButton *b = (QToolButton *)chld;
           hm = std::max(hm,b->size().width());
       }
   }
   foreach(chld,ui->mainToolBar->children())
   {
       if (!strcmp(chld->metaObject()->className(),"QToolButton"))
       {
           QToolButton *b = (QToolButton *)chld;
           b->setMinimumWidth(hm);
       }
   }
}


void MainWindow::loadSettings()
{
    QDate last = Settings.value("LastUpdate",QDate::currentDate().addDays(-11)).toDate();
    if (QDate::currentDate()>last.addDays(7))
    {
        QString upd = "\""+QApplication::applicationDirPath()+"\\gcupd.exe\" /autocheck";
        QProcess::startDetached(upd);
        Settings.setValue("LastUpdate",QDate::currentDate());
    }

    restoreState(Settings.value("MainWindow").toByteArray(),5);
    ui->actionEnable_Remote->setChecked(Settings.value("Remote",false).toBool());
    ui->lineEdit->setText(Settings.value("SrchFor","").toString());
}


void MainWindow::bringBack()
{
   showNormal();
   DWORD dwTimeout;
   SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &dwTimeout, 0);
   SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, 0);
   SetForegroundWindow((HWND)winId());
   SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)dwTimeout, 0);
}

void MainWindow::windowAdd(HWND win)
{
    int id;
    ui->tableWidget->insertRow(id = ui->tableWidget->rowCount());
    ui->tableWidget->setItem(id,0,new QTableWidgetItem(QString("%1").arg(id+1)));
    ui->tableWidget->setItem(id,1,new QTableWidgetItem(QString("%1").arg((DWORD)win,8,16,QChar('0'))));
    WList.push_back(win);
}

void MainWindow::on_pushButton_clicked()
{
    //сначала стоп скрипта, потом очистка! чтоб отработали стоп-функции
    on_actionStop_Script_triggered();
    WList.clear();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem(tr("#")));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem(tr("HWND")));
    ui->tableWidget->horizontalHeader()->setResizeMode( QHeaderView::Fixed );
    ui->tableWidget->horizontalHeader()->resizeSection(0,25);
    ui->tableWidget->horizontalHeader()->resizeSection(1,80);
}

HWND MainWindow::getWindow(unsigned long id)
{
    if (id<1 || id>WList.size())
        return NULL;
    return WList[id-1];
}

int MainWindow::getWinCount()
{
    return WList.size();

}

COLORREF MainWindow::getColor(HWND win, int x, int y)
{
    return QWindowTargeter::getColor(win,x,y);
}

DWORD MainWindow::setHotKey(DWORD mod, DWORD vkey)
{
    if (!vkey) return 0;
    DWORD id = makeHKId(mod,vkey);
    if (isIdRegistered(id))
        return id;
    if (RegisterHotKey((HWND)winId(), id, mod, vkey))
    {
        addHKId(id);
        return id;
    }
    return 0;
}

void  MainWindow::emulateHotKey(DWORD mod, DWORD vkey)
{
    PostMessageA(this->window()->winId(),WM_HOTKEY,0,((vkey&0xFFFF)<<16)+(mod&0xFFFF));
}

void MainWindow::unsetHKAll()
{
    unsetAllP( (HWND)winId() );
}

bool MainWindow::winEvent(MSG* message, long* result)
{
    if (message->message == WM_HOTKEY)
    {
        const DWORD vkey = HIWORD(message->lParam);
        const DWORD mod = LOWORD(message->lParam);
        DWORD id = makeHKId(mod,vkey);
        Script.hotKeyByUser(id);

        return true;
    }
    return false;
}

void MainWindow::on_actionStart_Script_triggered()
{
    if (ui->listWidget->currentItem())
        Script.runScriptFile(ui->listWidget->currentItem()->text());
}

void MainWindow::on_actionStop_Script_triggered()
{
    Script.stopNow();
}

void MainWindow::wait_forhk()
{
    ui->actionDummy->setIcon(QIcon(":/Icons/going"));
    this->setWindowIcon(QIcon(":/Icons/going"));
    ui->actionDummy->setText(tr("HotKeys are activated now."));
    ui->actionDummy->setEnabled(true);

    int cntr = Settings.value("DummyCntr",0).toInt();
    if (cntr<7)
    {
        QObject *chld;
        foreach(chld,ui->toolBar->children())
        {
            if (!strcmp(chld->metaObject()->className(),"QToolButton"))
            {
                QToolButton *b = (QToolButton *)chld;
                QPoint p( x()+
                          ui->toolBar->x()+
                          b->x()+b->width()/2,
                          y()+
                          ui->toolBar->y()+
                          b->y()+b->height()/2);
                QToolTip::showText(p,tr("Click button to see hotkeys' list."),b);
                break;
            }
        }
        cntr++;
    }
    Settings.setValue("DummyCntr",cntr);
}

void MainWindow::done_forhk()
{
    ui->actionDummy->setIcon(QIcon(":/Icons/stopped"));
    this->setWindowIcon(QIcon(":/Icons/stopped"));
    ui->actionDummy->setText(tr("HotKeys are deactivated now."));
    ui->actionDummy->setEnabled(false);
}

void ListDir(QString root, QString FileExt, QStringList &Res)
{
    QDir dir(root);
    dir.setFilter(QDir::NoSymLinks | QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
    QStringList filters;
    filters << FileExt;
    dir.setNameFilters(filters);
    dir.setSorting(QDir::Name);
    for (QStringList::const_iterator i = dir.entryList().constBegin();i!=dir.entryList().constEnd();i++)
    {
        Res.append(QString("%1/%2").arg(root).arg(*i));
    }

    QDir dir2(root);
    dir2.setFilter(QDir::NoSymLinks | QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
    for (QStringList::const_iterator i = dir2.entryList().constBegin();i!=dir2.entryList().constEnd();i++)
    {
        if (*i=="." || *i=="..") continue;
        ListDir(QString("%1/%2").arg(root).arg(*i),FileExt,Res);
    }
}


void MainWindow::on_actionReload_triggered()
{
    //reload scripts' list
    ui->listWidget->clear();
    QStringList fl;
    ListDir(QCoreApplication::applicationDirPath(),"*.lua",fl);
    if (getUserFolder()!="")
        ListDir(getUserFolder(),"*.lua",fl);
    ui->listWidget->addItems(fl);
    ui->actionStart_Script->setEnabled(false);
}

void MainWindow::on_listWidget_itemSelectionChanged()
{
    ui->actionStart_Script->setEnabled((bool)ui->listWidget->currentItem());
}

void MainWindow::hkTextsChanged()
{
    //new hotkey description added, update UI

}

void MainWindow::on_actionDummy_triggered()
{
    //show hk descriptions
    ShowHK hk(this);
    QString s = "";
    for (QHKTexts::iterator i = Texts.begin();i!=Texts.end();i++)
    {
        s = s +QString("<p><b>%1</b> - %2</p>").arg(i->first).arg(i->second.Text);
    }
    hk.setText(s);
    hk.exec();
}

void MainWindow::on_actionCheck_Updates_triggered()
{
    QString upd = "\""+QApplication::applicationDirPath()+"\\gcupd.exe\"";
    if (!QProcess::startDetached(upd))
    {
        QMessageBox::critical(this,windowTitle(),
          QString(tr("Cannot launch updater. Please start in manually. File is \"%1\"")).arg(upd));
    }
    else
    {
        Settings.setValue("LastUpdate",QDate::currentDate());
        this->close();
    }
}

QString MainWindow::getUserFolder(bool request)
{
    while(request && Settings.value("UserDir","").toString() == "")
       on_actionSet_Script_s_Folder_triggered();
    return Settings.value("UserDir","").toString();
}

void MainWindow::on_actionSet_Script_s_Folder_triggered()
{
    //add folder
    QString dir = QFileDialog::getExistingDirectory(this, tr("Set custom script's folder."),
                                                    getUserFolder(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (QCoreApplication::applicationDirPath()!=dir && dir!="")
    {
       Settings.setValue("UserDir",dir);
       on_actionReload_triggered();
    }
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QStringList sl;
    sl.push_back(index.data().toString());
    QProcess::startDetached("notepad.exe",sl);
    if (ui->actionDummy->isEnabled())
       QMessageBox::warning(this,windowTitle(),tr("Press start button again if you have changed running script."));
}

void MainWindow::on_actionWeb_Site_triggered()
{
    QDesktopServices::openUrl(QUrl("http://igra-em.ru/topics/igroshhelkatel"));
}

BOOL CALLBACK MainWindow::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
   MainWindow *This = (MainWindow*)lParam;
   if (This)
   {
       //qDebug()<<"aaa";
       QFixGG fg("GetWindowTextW","user32");
       wchar_t buf[2048];memset(buf,0,sizeof(buf));
       fg.call(3,(DWORD)hwnd,(DWORD)buf,2048);
       QString cap = QString::fromWCharArray(buf);
       //qDebug()<<cap;
       if (cap.contains(This->ui->lineEdit->text()))
       {
           bool hasone = false;
           for (QWndVector::iterator i=This->WList.begin();i!=This->WList.end();i++)
               if (hwnd == *i)
               {
                   hasone = true;
                   break;
               }
           if (!hasone)
              This->windowAdd(hwnd);
       }
       return true;
   }
   return false;
}

void MainWindow::on_pushButton_2_clicked()
{
    //find windows by title
    if (ui->lineEdit->text()=="")
    {
        QMessageBox::critical(this,windowTitle(),tr("Enter something to search for."));
    }
    else
    {
      Settings.setValue("SrchFor",ui->lineEdit->text());
      QFixGG gg("EnumWindows","user32");
      gg.call(2,EnumWindowsProc,(DWORD)this);
    }
}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    if (r>=0 && r<WList.size())
    {
        QFixGG g("SwitchToThisWindow","user32");
        g.call(2,WList[r],1);
    }

}
