#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <vector>
#include <QSettings>

#include "qluascript.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow, public virtual QScriptHelper
{
    Q_OBJECT

public:
    typedef std::vector<HWND> QWndVector;
    explicit MainWindow(QWidget *parent = 0);
    virtual HWND getWindow(unsigned long id);
    virtual int  getWinCount();
    virtual DWORD setHotKey(DWORD mod, DWORD vkey);
    virtual void  unsetHKAll();
    virtual COLORREF getColor(HWND win, int x, int y);
    virtual QString getUserFolder(bool request=false);
    virtual void  emulateHotKey(DWORD mod, DWORD vkey);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWndVector WList;
    QLuaScript Script;
    //QLabel *lState;
    bool winEvent(MSG* message, long* result);
    QSettings Settings;
    void loadSettings();
    void setupButtons();
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

protected:
    virtual void hkTextsChanged();    
private slots:
    void windowAdd(HWND win);
    void on_pushButton_clicked();
    void on_actionStart_Script_triggered();
    void on_actionStop_Script_triggered();
    void wait_forhk();
    void done_forhk();
    void bringBack();
    void on_actionReload_triggered();
    void on_listWidget_itemSelectionChanged();
    void on_actionDummy_triggered();
    void on_actionCheck_Updates_triggered();
    void on_actionSet_Script_s_Folder_triggered();
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_actionWeb_Site_triggered();
    void on_pushButton_2_clicked();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
