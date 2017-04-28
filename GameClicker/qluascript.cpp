#include <QDebug>
#include "qluascript.h"
#include <QtWidgets//QMessageBox>
#include "mainwindow.h"
#include <QKeySequence>
#include <QKeyEvent>
#include <qwindowtargeter.h>
#include "Versions.h"
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QFileDialog>

enum user_gets{GETINT,GETNUMBER,GETSTRING,GETLIST,GETFOLDER};
int getApiVersion(lua_State *L)
{
    lua_pushnumber(L,GCAPI_VER);
    return 1;
}


void foreground(HWND win, lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);
    const QFixGG* g = s->getGuard("SwitchToThisWindow","user32");
    /*
    DWORD dwTimeout;
    SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &dwTimeout, 0);
    SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, 0);
    SetForegroundWindow(win);
    SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)dwTimeout, 0);
*/
     g->call(2,win,1);
     QCoreApplication::processEvents(QEventLoop::AllEvents,50);
}

HWND check1stParam(lua_State *L)
{
    int n = lua_gettop(L);
    QLuaScript *s = QLuaScript::getThis(L);
    HWND res = NULL;
    if (n<1 || !lua_isnumber(L,1) || !(res=s->getWindow(lua_tointeger(L,1))))
    {
        lua_pushstring(L, "1st parameter does not references to valid window in list.");
        lua_error(L);
    }
    return res;
}

int addWindowsHandler(lua_State *L)
{
    int n = lua_gettop(L);
    if (!n || !lua_isstring(L,1))
    {
        lua_pushstring(L, "addWindowsHandler has 1 parameter which is string.");
        lua_error(L);
    }
    QLuaScript *s = QLuaScript::getThis(L);
    s->addWinHandler(L,QString::fromLocal8Bit(lua_tostring(L,1)));
    return 0;
}

int rmWindowsHandler(lua_State *L)
{
    int n = lua_gettop(L);
    if (!n || !lua_isstring(L,1))
    {
        lua_pushstring(L, "rmWindowsHandler has 1 parameter which is string.");
        lua_error(L);
    }
    QLuaScript *s = QLuaScript::getThis(L);
    s->removeWinHandler(L,QString::fromLocal8Bit(lua_tostring(L,1)));
    return 0;
}

void translateCoords(HWND win, int* x, int* y)
{
    //translates win coordinates into global screen
    RECT wr;
    GetWindowRect(win,&wr);
    *x += wr.left;
    *y += wr.top;
}


bool isActiveWin(lua_State *L, HWND win)
{
    QLuaScript *s = QLuaScript::getThis(L);
    const QFixGG* g = s->getGuard("GetForegroundWindow","user32");
    return (g->call(0) == (unsigned long)win);
}


int getHWND(lua_State *L)
{
    lua_pushinteger(L,(DWORD)check1stParam(L));
    return 1;
}

int getColor(lua_State *L)
{
    HWND win = check1stParam(L);
    if (!lua_isnumber(L,2) || !lua_isnumber(L,3))
    {
       lua_pushstring(L, "2nd & 3rd parameters have to be x,y in getColor.");
       lua_error(L);
    }
    //QLuaScript *s = QLuaScript::getThis(L);
    lua_pushinteger(L,QWindowTargeter::getColor(win,lua_tointeger(L,2),lua_tointeger(L,3)));
    return 1;
}

int textOut(lua_State *L)
{
    HWND win = check1stParam(L);

    if (!lua_isnumber(L,2) || !lua_isnumber(L,3) || !lua_isstring(L,4))
    {
       lua_pushstring(L, "2nd & 3rd parameters have to be x,y; 4th is text in textOut.");
       lua_error(L);
    }
    int x = lua_tonumber(L,2);
    int y = lua_tonumber(L,3);
    translateCoords(win,&x,&y);

    QString txt = QString::fromLocal8Bit(lua_tostring(L,4));

    HDC dc = GetDC(win);
    if (dc)
    {
        SetTextColor(dc,RGB(255,255,255));
        SetBkMode(dc,TRANSPARENT);
        TextOut(dc,x,y,txt.toStdWString().c_str(),txt.length());
        ReleaseDC(win,dc);
    }
    return 0;
}

int mouseLClick(lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);
    HWND win = check1stParam(L);
    if (!isActiveWin(L,win)) //do nothing if window is not active
    {
        //qDebug()<<"SendInput - not active win, skip.";
        return 0;
    }
    if (!lua_isnumber(L,2) || !lua_isnumber(L,3))
    {
       lua_pushstring(L, "2nd & 3rd parameters have to be x,y;");
       lua_error(L);
    }
    int x = lua_tonumber(L,2);
    int y = lua_tonumber(L,3);
    translateCoords(win,&x,&y);

    float vscreen_x = 65535. / GetSystemMetrics(SM_CXVIRTUALSCREEN);
    float vscreen_y = 65535. / GetSystemMetrics(SM_CYVIRTUALSCREEN);

    INPUT mi[3];
    mi[0].type = mi[1].type = mi[2].type = INPUT_MOUSE;


    mi[0].mi.dx = x*vscreen_x;
    mi[0].mi.dy = y*vscreen_y;

    mi[1].mi.dx = mi[2].mi.dx = 0;
    mi[1].mi.dy = mi[2].mi.dy = 0;

    //qDebug()<<mi[0].mi.dx<<" "<<mi[0].mi.dy;
    mi[0].mi.mouseData = mi[1].mi.mouseData = mi[2].mi.mouseData = 0;

    mi[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE |0x4000 | MOUSEEVENTF_MOVE;
    mi[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    mi[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    mi[0].mi.time = mi[1].mi.time = mi[2].mi.time = 0;
    mi[2].mi.dwExtraInfo = mi[0].mi.dwExtraInfo = mi[1].mi.dwExtraInfo = NULL;

    const QFixGG* g = s->getGuard("SendInput","user32");
    g->call(3,3,mi,sizeof(INPUT));
    return 0;
}

int sendCharW(lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);

    if (!lua_isstring(L,2))
    {
        lua_pushstring(L, "2nd parameter must be 1 symbol string");
        lua_error(L);
    }
    const QFixGG* g = s->getGuard("SendMessageW","user32");
    DWORD wp = QString::fromLocal8Bit(lua_tostring(L,2)).utf16()[0];

    g->call(4,check1stParam(L),WM_CHAR,wp,0);

    return 0;
}

int sendCharA(lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);

    if (!lua_isstring(L,2))
    {
        lua_pushstring(L, "2nd parameter must be 1 symbol string");
        lua_error(L);
    }
    const QFixGG* g = s->getGuard("SendMessageA","user32");
    const char *ls = lua_tostring(L,2);
    DWORD wp = ls[0] & 0xFF;

    g->call(4,check1stParam(L),WM_CHAR,wp,0);

    return 0;
}


int postCharW(lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);

    if (!lua_isstring(L,2))
    {
        lua_pushstring(L, "2nd parameter must be 1 symbol string");
        lua_error(L);
    }
    const QFixGG* g = s->getGuard("PostMessageW","user32");
    DWORD wp = QString::fromLocal8Bit(lua_tostring(L,2)).utf16()[0];

    g->call(4,check1stParam(L),WM_CHAR,wp,0);


    return 0;
}

int postCharA(lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);

    if (!lua_isstring(L,2))
    {
        lua_pushstring(L, "2nd parameter must be 1 symbol string");
        lua_error(L);
    }
    const QFixGG* g = s->getGuard("PostMessageA","user32");
    const char *ls = lua_tostring(L,2);
    DWORD wp = ls[0] & 0xFF;

    g->call(4,check1stParam(L),WM_CHAR,wp,0);

    return 0;
}

int sendMessageW(lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);
    const QFixGG* g = s->getGuard("SendMessageW","user32");
    //int msg = lua_tointeger(L,2);
    int wp = 0, lp =0;
    QString wps;QString lps;
    if (lua_isnumber(L,3))
        wp = lua_tointeger(L,3);
    else
    if (lua_isstring(L,3))
    {
        wps = QString::fromLocal8Bit(lua_tostring(L,3));
        wp = (DWORD)wps.utf16();
    }

    if (lua_isnumber(L,4))
        lp = lua_tointeger(L,4);
    else
    if (lua_isstring(L,4))
    {
        lps = QString::fromLocal8Bit(lua_tostring(L,4));
        lp = (DWORD)lps.utf16();
    }

    HWND t = check1stParam(L);
    g->call(4,t,lua_tointeger(L,2),wp,lp);

    return 0;
}

int postMessageW(lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);
    const QFixGG* g = s->getGuard("PostMessageW","user32");
    //int msg = lua_tointeger(L,2);
    int wp = 0, lp =0;
    QString wps;QString lps;
    if (lua_isnumber(L,3))
        wp = lua_tointeger(L,3);
    else
    if (lua_isstring(L,3))
    {
        wps = QString::fromLocal8Bit(lua_tostring(L,3));
        wp = (DWORD)wps.utf16();
    }

    if (lua_isnumber(L,4))
        lp = lua_tointeger(L,4);
    else
    if (lua_isstring(L,4))
    {
        lps = QString::fromLocal8Bit(lua_tostring(L,4));
        lp = (DWORD)lps.utf16();
    }


    HWND t = check1stParam(L);
    g->call(4,t,lua_tointeger(L,2),wp,lp);

    return 0;
}


void lua_DbgHookMain(lua_State *L, lua_Debug *ar)
{
    //main script is looped
    QCoreApplication::processEvents(QEventLoop::AllEvents,50);

}

int dbgyield(lua_State *L)
{
    qDebug()<<"Dbg hook2" <<QTime::currentTime();
    return 0;
}


void lua_DbgHook(lua_State *L, lua_Debug *ar)
{
    QCoreApplication::processEvents(QEventLoop::AllEvents,50);
    //static int perf = 0;
    //if (++perf<10)
      //  return;
    //perf = 0;
    //qDebug()<<"Dbg hook" <<QTime::currentTime();
    QLuaScript *s = QLuaScript::getThis(L);
    if (!s->shouldYield(L))
        return;
    //call as "delay(10)"
    //qDebug()<<"Dbg hook, yielding" <<QTime::currentTime()<<L;
    lua_yieldk(L,0,0,dbgyield);
    //походу первое срабатывание отдает в resume указанную функцию, которую потом резум должен
    //вызвать. Но, мы етого делать не бум.
}


int registerHK(lua_State *L)
{
    int n = lua_gettop(L);

    if (n<2 || !lua_isstring(L,1) || !lua_isstring(L,2))
    {
        lua_pushstring(L, "registerHotKey has 2-3 parameters which are strings.");
        lua_error(L);
    }
    QString seq  = QString::fromLocal8Bit(lua_tostring(L,1));
    QString func = QString::fromLocal8Bit(lua_tostring(L,2));
    QString text = "";
    if (n>2 && lua_isstring(L,3))
    {
        text = QString::fromLocal8Bit(lua_tostring(L,3));
    }
    QLuaScript *s = QLuaScript::getThis(L);
    if (!s->setHotKey(seq,func,text))
    {
        QString s = QString(QObject::tr("Keystroke \"%1\" (%2) is wrong or used already.")).arg(seq).arg(func);
        lua_pushstring(L, s.toLocal8Bit().data());
        lua_error(L);
    }
    return 0;
}

int emulateHK(lua_State *L)
{
    int n = lua_gettop(L);

    if (n<1 || !lua_isstring(L,1))
    {
        lua_pushstring(L, "emulateHotKey has 1 parameter which is string.");
        lua_error(L);
    }
    QString seq  = QString::fromLocal8Bit(lua_tostring(L,1));
    QLuaScript *s = QLuaScript::getThis(L);
    s->emulateHotKey(seq);
    return 0;
}

int addStopHandler(lua_State *L)
{
    int n = lua_gettop(L);
    if (!n || !lua_isstring(L,1))
    {
        lua_pushstring(L, "addStopHandler has 1 parameter which is string.");
        lua_error(L);
    }
    QLuaScript *s = QLuaScript::getThis(L);
    s->addStopHandler(L,QString::fromLocal8Bit(lua_tostring(L,1)));
    return 0;
}

int getHKCounter(lua_State *L)
{
    int n = lua_gettop(L);

    if (n<1 || !lua_isstring(L,1))
    {
        lua_pushstring(L, "getHKCounter has 1 parameter which is string.");
        lua_error(L);
    }
    QString func = QString::fromLocal8Bit(lua_tostring(L,1));
    QLuaScript *s = QLuaScript::getThis(L);
    //qDebug()<<"Cnt: "<<s->getHKCounter(func);
    lua_pushinteger(L,s->getHKCounter(func));
    return 1;
}

int getWinCount(lua_State *L)
{
   QLuaScript *s = QLuaScript::getThis(L);
   lua_pushinteger(L,s->getWinCount());
   return 1;
}

int showMsg(lua_State *L)
{
    int n = lua_gettop(L);
    if (n<1 || !lua_isstring(L,1))
    {
        lua_pushstring(L, "showWsg has at least 1 parameter which is string.");
        lua_error(L);
    }
    QLuaScript *s = QLuaScript::getThis(L);
    QWidget *MainWin = s->getOwner();

    QMessageBox msgBox(MainWin);
    msgBox.setText(QString::fromLocal8Bit(lua_tostring(L,1)));
    msgBox.setWindowTitle(QObject::tr("Script's Message"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
    return 0;
}

int qdebug(lua_State *L)
{
    int n = lua_gettop(L);
    if (n<1 || !lua_isstring(L,1))
    {
        lua_pushstring(L, "qdebug has at least 1 parameter which is string.");
        lua_error(L);
    }
    qDebug()<<QString::fromLocal8Bit(lua_tostring(L,1));
    return 0;
}

int activateWin(lua_State* L)
{
    foreground(check1stParam(L),L);
    return 0;
}

int isActiveWin(lua_State* L)
{
    HWND win = check1stParam(L);
    lua_pushboolean(L,isActiveWin(L,win));
    return 1;
}

int getUserFolder(lua_State* L)
{
    QLuaScript *s = QLuaScript::getThis(L);
    int n = lua_gettop(L);
    bool amust = false;
    if (n && lua_isboolean(L,1))
    {
        amust = lua_toboolean(L,1);
    }
    QString f = s->getUserFolder(amust);
    lua_pushstring(L,f.toLocal8Bit().data());
    return 1;
}

int callDllA(lua_State* L)
{
    int n = lua_gettop(L);
    if (n<2 || !lua_isstring(L,1) || !lua_isstring(L,2))
    {

            lua_pushstring(L, "callDllA has at least 2 parameters strings: function name and dll name.");
            lua_error(L);
    }
    QLuaScript *s = QLuaScript::getThis(L);

    const QFixGG *g = s->getGuard(QString::fromLocal8Bit(lua_tostring(L,1)), //function
                                  QString::fromLocal8Bit(lua_tostring(L,2))); //dll name
    if (!g->isready())
    {
        char err[512];memset(err,0,sizeof(err));
        sprintf(err,"callDllA cannot load \"%s\" from \"%s\".",lua_tostring(L,1),lua_tostring(L,2));
        lua_pushstring(L,err);
        lua_error(L);
    }
    int r = g->callfromlua(L);
    int e = GetLastError();
   // qDebug()<<r;
    lua_pushnumber(L,r);
    lua_pushnumber(L,e);
    return 2;
}

int derefAsDWORD(lua_State *L)
{
    DWORD res = 0;
    int n = lua_gettop(L);
    if (n && lua_isnumber(L,1))
    {
        DWORD *p = (DWORD*)lua_tointeger(L,1);
        if (p)
            res = *p;
    }
    lua_pushinteger(L,res);
    return 1;
}

int derefAsBYTE(lua_State *L)
{
    BYTE res = 0;
    int n = lua_gettop(L);
    if (n && lua_isnumber(L,1))
    {
        BYTE *p = (BYTE*)lua_tointeger(L,1);
        if (p)
            res = *p & 0xFF;
    }
    lua_pushinteger(L,res);
    return 1;
}

int startCritical(lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);
    s->setCritical(L,true);
    return 0;
}

int stopCritical(lua_State *L)
{
    QLuaScript *s = QLuaScript::getThis(L);
    s->setCritical(L,false);
    return 0;
}

int lua_userInput(lua_State *L)
{
   QLuaScript *s = QLuaScript::getThis(L);
   QWidget *MainWin = s->getOwner();

    //getting input from user
   int n = lua_gettop(L); //amount of parameters
   if (n<3)
   {
       lua_pushstring(L, "userInput must have at least 3 parameters.");
       lua_error(L);
   }
   if (!lua_isnumber(L, 1) || !lua_isstring(L, 2) || !lua_isstring(L, 3))
   {
       lua_pushstring(L, "userInput 1st parameter should be mode number, 2nd and 3rd strings.");
       lua_error(L);
   }
   QString title = QString::fromLocal8Bit(lua_tostring(L,2));
   QString label = QString::fromLocal8Bit(lua_tostring(L,3));
   bool ok = false;

   switch(lua_tointeger(L,1))
   {
     case GETINT:
     {
       long def = 0;
       long min = -2147483647;
       long max =  2147483647;
       long step = 1;
       if (n>3 && lua_isnumber(L, 4)) def  = lua_tointeger(L, 4);
       if (n>4 && lua_isnumber(L, 5)) min  = lua_tointeger(L, 5);
       if (n>5 && lua_isnumber(L, 6)) max  = lua_tointeger(L, 6);
       if (n>6 && lua_isnumber(L, 7)) step = lua_tointeger(L, 7);
       long i = QInputDialog::getInt(MainWin,title, label, def, min, max, step, &ok);
       lua_pushboolean(L,ok);
       lua_pushinteger(L,i);
       return 2;
     }
     break;
     case GETSTRING:
     {
       QString def = "";
       if (n>3 && lua_isstring(L, 4)) def  = QString::fromLocal8Bit(lua_tostring(L, 4));
       QString i = QInputDialog::getText(MainWin,title, label, QLineEdit::Normal, def, &ok);
       lua_pushboolean(L,ok);
       lua_pushstring(L,i.toLocal8Bit().data());
       return 2;
     }
     break;
     case GETLIST:
     {
       QStringList options;
       long selected=0;
       bool editable = false;
       if (n<4 || !lua_istable(L,4))
       {
           lua_pushstring(L, "userInput/GETLIST must have table of options as 4th parameter.");
           lua_error(L);
       }
       if (n>4 && lua_isnumber(L, 5))  selected  = lua_tointeger(L, 5);
       if (n>5 && lua_isboolean(L, 6)) editable  = lua_toboolean(L, 6);

       lua_pushnil(L);  /* first key */
       while (lua_next(L, 4) != 0)
       {
          /* uses 'key' (at index -2) and 'value' (at index -1) */
           if (lua_isstring(L,-1))
               options.append(QString::fromLocal8Bit(lua_tostring(L,-1)));
          /* removes 'value'; keeps 'key' for next iteration */
          lua_pop(L, 1);
       }
       QString i = QInputDialog::getItem(MainWin,title, label, options, selected-1, editable, &ok);
       lua_pushboolean(L,ok);
       lua_pushstring(L,i.toLocal8Bit().data());
       return 2;
     }
     break;
     case GETFOLDER:
     {
       QString capt = QObject::tr("Select folder");
       QString def  = ".";
       if (n>3 && lua_isstring(L,4))
           capt = QString::fromLocal8Bit(lua_tostring(L,4));
       if (n>4 && lua_isstring(L,5))
           def = QString::fromLocal8Bit(lua_tostring(L,5));
       QString dir = QFileDialog::getExistingDirectory(MainWin, capt, def, QFileDialog::ShowDirsOnly);
       lua_pushstring(L,dir.toLocal8Bit().data());
       return 1;
     }
     break;
     default:
         lua_pushstring(L, "userInput 1st parameter is unknown mode.");
         lua_error(L);
         break;
   }
   return 0;
}


QLuaScript::QThisFinder QLuaScript::thisFinder;
QLuaScript::QLuaScript(QScriptHelper *h, QObject *parent):QObject(parent)
{
    Helper  = h;
    luaVM   = NULL;
    stoppingnow = false;

}

QLuaScript::~QLuaScript()
{
    closeVM(true);
}

QWidget*  QLuaScript::getOwner()
{
    return (QWidget*)this->parent();
}

bool QLuaScript::setHotKey(QString seq, QString func, QString text)
{
    DWORD mod = 0;
    DWORD vkey = 0;
    mapQtKey2Win(seq,&mod,&vkey);

    DWORD id = Helper->setHotKey(mod,vkey);
    if (id && !Functions.count(id))
    {
        Functions[id].luaFunc = func;
        Functions[id].clear();
        Helper->addHKText(seq,func,text);
        return true;
    }
    return false;
}

void QLuaScript::emulateHotKey(QString hk)
{
    DWORD mod = 0;
    DWORD vkey = 0;
    mapQtKey2Win(hk,&mod,&vkey);
    Helper->emulateHotKey(mod,vkey);
}

quint32 QLuaScript::getHKCounter(QString func)
{
    quint32 res = 0;
    for (QCoMap::iterator i=Functions.begin();i!=Functions.end();i++)
    {
        if (i->second.luaFunc == func)
        {
            res = i->second.hcounter;
            break;
        }
    }
    return res;
}


void QLuaScript::hotUserCntr(DWORD id)
{
    if (Functions.count(id))
    {
        Functions[id].hcounter++;
    }
}

void QLuaScript::setCritical(lua_State* L, bool enable)
{

    DWORD id = getIdByState(L);
    if (Functions.count(id) && Functions[id].thread == L)
    {
        Functions[id].critical = enable;
        if (!enable)
           Functions[id].criticalcounter = 0;
    }

}

bool QLuaScript::shouldYield(lua_State* L)
{
    bool res = (L!=luaVM);
    DWORD id = getIdByState(L);
    if (Functions.count(id) && Functions[id].thread == L)
    {
        res = Functions[id].suspendedTill+2000 < GetTickCount();
    }
    return res;
}

void QLuaScript::threadsLoop()
{
    static bool entryonce = true;
    if (entryonce)
    {
       entryonce = false;
       started();
       while (true)
       {
          if (Yielded.size())
          {
            DWORD id = Yielded.front();
            Yielded.pop_front();
            //qDebug()<<"id: "<<id<<QTime::currentTime();
            if (id == STOP_HOTKEY)
                break;

            if (Functions.count(id) && Functions[id].thread)
            {
                QThreadState res = THSWAITYET;
                if(Functions[id].suspendedTill<GetTickCount())
                {
                     res = runThread(id);
                }
                if (THSERROR == res)
                    break;

                if (THSSUSPENDED == res || THSWAITYET == res)
                {
                   if (Functions[id].critical && Functions[id].criticalcounter < 10 )
                   {
                       if (THSSUSPENDED == res)
                           Functions[id].criticalcounter++;
                       Yielded.push_front(id);
                   }
                   else
                       Yielded.push_back(id);
                }
            }
          }
          Sleep(10);
          QCoreApplication::processEvents(QEventLoop::AllEvents);
       }
       entryonce = true;
       closeVM(true);
       stopped();
    }
}

QLuaScript::QThreadState QLuaScript::runThread(DWORD id)
{
    if (!Functions.count(id))
        return THSERROR;
    QHotSignal& s = Functions[id];
    if (!s.thread)
        return THSERROR;
    //qDebug()<<"Resuming "<<s.thread<<QTime::currentTime();
    int err = lua_resume(s.thread,s.thread,0);

    //int err = lua_resume(s.thread,NULL,0);
    if (err && err!=LUA_YIELD)
    {
       luaerror(s.thread);//just done with script - thread error
       destroyThread(id);
       return THSERROR;
    }
    if (err == 0)
    {
        destroyThread(id);
        return THSDONE;
    }
    else
    {
        int n = lua_gettop(s.thread);
        int ms = 10;
        if (n==1)
            ms = lua_tointeger(s.thread,-1);
        //qDebug()<<"Paused for "<<ms<<n<<s.thread;
        if (n)
          lua_pop(s.thread,n);
        s.suspendedTill = GetTickCount()+ms;
        return THSSUSPENDED;
    }
    return THSERROR;
}

bool QLuaScript::createThread(DWORD id)
{
    if (Functions.count(id) && luaVM && !Functions[id].thread)
    {
        Functions[id].clear();
        Functions[id].suspendedTill = 0;
        Functions[id].thread  = lua_newthread(luaVM);
        //luaL_dostring(Functions[id].thread, "debug.sethook(dbgyield,'',1000)");
        Functions[id].hcounter = 1;
        Reverse[Functions[id].thread] = id;

        //prevent thread from being GC
        //pushing thread into registry table. it's ID will be a key and value
        lua_pushlightuserdata(luaVM, (void*)lua_topointer(luaVM, -1));
        lua_pushvalue(luaVM, -2);
        lua_settable(luaVM, LUA_REGISTRYINDEX);

        lua_sethook (Functions[id].thread, lua_DbgHook, LUA_MASKCOUNT, 1000);
        lua_getglobal(Functions[id].thread,Functions[id].luaFunc.toLocal8Bit().data());

        addThis(Functions[id].thread,this);
        return true;
    }
    return false;
}

void QLuaScript::destroyThread(DWORD id)
{
    if (!Functions.count(id))
        return;

    lua_State* pThread = Functions[id].thread;
    Reverse.erase(pThread);

    if (pThread && luaVM)
    {
      delThis(pThread);
      lua_State* p;
      int n = lua_gettop(luaVM);
      for (int i(1); i<=n; ++i)
         if (lua_isthread(luaVM, i))
         {
              p = lua_tothread(luaVM, i);
              if (p==pThread)
              {
                   lua_remove(luaVM, n-i-1);
                   qDebug()<<"Deleting thread: "<<pThread;
                   break;
              }
         }
      //allow to GC this thread
      lua_pushlightuserdata(luaVM, (void*)lua_topointer(luaVM, -1));
      lua_pushlightuserdata(luaVM, NULL);
      lua_settable(luaVM, LUA_REGISTRYINDEX);
    }
    Functions[id].clear();
}

void QLuaScript::hotKeyByUser(DWORD id)
{
    if (createThread(id))
        Yielded.push_back(id);
    else
        hotUserCntr(id);
}

void QLuaScript::clearHotSignals()
{
    Functions.clear();
    Yielded.clear();
}

HWND QLuaScript::getWindow(unsigned long num)
{
    HWND w = Helper->getWindow(num);
    if (w)
      processWinHandlers(num);
    return w;
}

int QLuaScript::getWinCount()
{
    return Helper->getWinCount();
}

QLuaScript* QLuaScript::getThis(lua_State* L)
{
    if (thisFinder.count(L) && thisFinder[L])
    {
        return thisFinder[L];
    }
    lua_pushstring(L, "Script object does not exists for VM. You should not see this error.");
    lua_error(L);
    return NULL;
}

const QFixGG* QLuaScript::getGuard(QString Func, QString Library)
{
    if (!Guards.count("p"+Func+Library))
    {
        QFixGG g(Func,Library);
        Guards["p"+Func+Library] = g;
    }
    QFixGG &r = Guards["p"+Func+Library];
    return &r;
}

void QLuaScript::addThis(lua_State* L, QLuaScript* p)
{
    thisFinder[L] = p;
}

void QLuaScript::delThis(lua_State* L)
{
    if (thisFinder.count(L))
      thisFinder.erase(L);
}

void QLuaScript::closeVM(bool a)
{
    Helper->unsetHKAll();
    clearHotSignals();
    processStopHandlers();

    if (luaVM)
    {
        lua_close(luaVM);
        delThis(luaVM);
    }
    Guards.clear();
    luaVM = NULL;
}

void QLuaScript::stopNow()
{
    Yielded.push_back(STOP_HOTKEY);
}


lua_State* QLuaScript::getVM()
{
    return luaVM;
}

void QLuaScript::luaerror()
{
    luaerror(luaVM);
}

void QLuaScript::luaerror(lua_State* L)
{
    setCritical(L,false);
    QMessageBox msgBox;
    msgBox.setText(tr("Error while executing script."));
    msgBox.setWindowTitle(tr("LUA Script Error"));
    msgBox.setDetailedText(lua_tostring(L,-1));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
    lua_pop(L,1);
    stopNow();
}


void QLuaScript::luaSetupGlobals()
{
    //#define g_pushint(A) lua_pushinteger(luaVM,(A));lua_setfield(luaVM, LUA_GLOBALSINDEX,#A)
    #define g_pushint(A) lua_pushinteger(luaVM,(A));lua_setglobal(luaVM,#A)
    g_pushint(GETINT);
    g_pushint(GETSTRING);
    g_pushint(GETLIST);
    g_pushint(GETFOLDER);

    lua_register(luaVM,"dbgyield",       dbgyield);
    lua_register(luaVM,"userInput",      lua_userInput);
    lua_register(luaVM,"sendCharA",      sendCharA);
    lua_register(luaVM,"sendChar",       sendCharW);

    lua_register(luaVM,"postCharA",      postCharA);
    lua_register(luaVM,"postChar",       postCharW);
    lua_register(luaVM,"sendMessage",    sendMessageW);
    lua_register(luaVM,"postMessage",    postMessageW);
    lua_register(luaVM,"registerHotKey", registerHK);
    lua_register(luaVM,"getWinCount",    ::getWinCount);
    lua_register(luaVM,"showMsg",        showMsg);
    lua_register(luaVM,"activateWin",    activateWin);
    lua_register(luaVM,"isActiveWin",    isActiveWin);
    lua_register(luaVM,"getHWND",        getHWND);
    lua_register(luaVM,"getColor",       getColor);
    lua_register(luaVM,"getPressCount",  ::getHKCounter);
    lua_register(luaVM,"getApiVersion",  getApiVersion);
    lua_register(luaVM,"textOut",        textOut);
    lua_register(luaVM,"mouseLClick",    mouseLClick);
    lua_register(luaVM,"callDllA",       callDllA);
    lua_register(luaVM,"getUserFolder",  ::getUserFolder);
    lua_register(luaVM,"derefAsDWORD",   derefAsDWORD);
    lua_register(luaVM,"derefAsBYTE",    derefAsBYTE);
    lua_register(luaVM,"addStopHandler", ::addStopHandler);
    lua_register(luaVM,"startCritical",  startCritical);
    lua_register(luaVM,"stopCritical",   stopCritical);
    lua_register(luaVM,"addWindowsHandler",addWindowsHandler);
    lua_register(luaVM,"rmWindowsHandler",rmWindowsHandler);
    lua_register(luaVM,"emulateHotKey",  emulateHK);
    lua_register(luaVM,"qdebug",         qdebug);

    //need "sheduled hotkey" for infinite loops

    QString pt = QString("%2/?.lua;%2/?.inc;%2/?;./?;./?.lua;%1/?;%1/?.lua;%1/inc/?;%1/inc/?.lua;%1/inc/?.inc;").arg(QCoreApplication::applicationDirPath()).arg((Helper->getUserFolder()!="")?Helper->getUserFolder():".");
/*
    lua_pushstring(luaVM, "package");
    //lua_gettable(luaVM, LUA_GLOBALSINDEX);
    //чета тут ошибка в новой луа, нуль
    lua_pushstring(luaVM, "path");
    lua_pushstring(luaVM, pt.toLocal8Bit().data());
    lua_settable(luaVM, -3);
*/
    lua_getglobal( luaVM, "package" );
    lua_pushstring(luaVM, pt.toLocal8Bit().data());
    lua_setfield(luaVM,-2,"path");
    lua_pop(luaVM,1);//remove global table from stack

    lua_sethook (luaVM, lua_DbgHookMain, LUA_MASKCOUNT, 100);
    luaL_dostring(luaVM,"function dhdhdhd()\ncoroutine.yield(0);\nend;");

}



#ifdef LUA_TEST_LEAKS
int foo(lua_State*L) {
      Obj obj;
      return luaL_error(L, "error in foo");
  }
#endif



void QLuaScript::reRunRun()
{
   runScript(rerun_fn,rerun_is);
}

bool QLuaScript::runScript(QString Script,bool is_filename)
{
    stopNow();
    if (luaVM)
    {
        rerun_fn = Script;
        rerun_is = is_filename;
        QTimer::singleShot(100,this,SLOT(reRunRun()));
        return false;
    }
    clearHotSignals();
    luaVM = luaL_newstate();


    if (Script == "")
    {
        Script = "require \"main\"";
        is_filename = false;
    }
    if (luaVM)
    {
        stoppingnow = false;
        luaL_openlibs(luaVM);
//------------------------TEST ERRORS-------------------
#ifdef LUA_TEST_LEAKS
        lua_pushcfunction(luaVM, foo);
                int r = lua_pcall(luaVM, 0,0,0);

                if (r == LUA_ERRRUN) {
                    qDebug() << lua_tostring(luaVM, -1);
                }

                lua_close(luaVM);
                return false;
#endif
//------------------------------------------------------
        luaSetupGlobals();
        if (is_filename)
        {
            if (luaL_loadfile(luaVM,Script.toLocal8Bit().data()))
            {
                    luaerror();
                    return false;
            }
        }
        else
        {
          if (luaL_loadstring(luaVM,Script.toLocal8Bit().data()))
          {
              luaerror();
              return false;
          }
        }
    }
    else return false;
    addThis(luaVM,this);

    int err = 0;
    if ((err = lua_pcall(luaVM,0,0,0)))
    {
        luaerror();
        closeVM(true);
        return false;
    }
    QTimer::singleShot(10,this,SLOT(threadsLoop()));
    return true;
}

bool QLuaScript::runScriptFile(QString Script)
{
    return runScript(Script,true);
}

void QLuaScript::mapQtKey2Win(QString seq,DWORD *mod,DWORD *vkey)
{
    *mod = 0;
    *vkey = 0;
    QKeySequence ks(seq,QKeySequence::NativeText);
    unsigned int key = ks[0];

    if ((Qt::CTRL  & key)  == Qt::CTRL)
        *mod+=MOD_CONTROL;
    if ((Qt::SHIFT & key)  == Qt::SHIFT)
        *mod+=MOD_SHIFT;
    if ((Qt::ALT   & key)  == Qt::ALT)
        *mod+=MOD_ALT;
    if ((Qt::META  & key)  == Qt::META)
        *mod+=MOD_WIN;

    key &=~Qt::KeyboardModifierMask;
    if ((Qt::Key_Escape & key) == Qt::Key_Escape) //ESC последовательность
    {
            using namespace Qt;
            const DWORD kmap[]={
                Key_Escape,VK_ESCAPE,
                Key_Tab,VK_TAB,
                Key_Backspace,VK_BACK,
                Key_Return,VK_RETURN,
                Key_Enter,VK_RETURN,
                Key_Insert,VK_INSERT,
                Key_Delete,VK_DELETE,
                Key_Pause,VK_PAUSE,
                Key_Print,VK_PRINT,
                Key_Home,VK_HOME,
                Key_End,VK_END,
                Key_Left,VK_LEFT,
                Key_Up,VK_UP,
                Key_Right,VK_RIGHT,
                Key_Down,VK_DOWN,
                Key_PageUp,VK_PRIOR,
                Key_PageDown,VK_NEXT,
                Key_CapsLock,VK_CAPITAL,
                Key_NumLock,VK_NUMLOCK,
                Key_ScrollLock,VK_SCROLL,
                Key_F1,VK_F1,                // function keys
                Key_F2,VK_F2,
                Key_F3,VK_F3,
                Key_F4,VK_F4,
                Key_F5,VK_F5,
                Key_F6,VK_F6,
                Key_F7,VK_F7,
                Key_F8,VK_F8,
                Key_F9,VK_F9,
                Key_F10,VK_F10,
                Key_F11,VK_F11,
                Key_F12,VK_F12,
                0xFFFFFFFF,0xFFFFFFFF
            };
            for (int i=0;kmap[i]!=0xFFFFFFFF;i+=2)
            {
                if (key  == kmap[i])
                {
                    *vkey = kmap[i+1];
                    break;
                }
            }

    }
    else
    {
        short vk = VkKeyScan(key & 0xFFFF);//looks like it will be unicode char
        if (vk != -1)
            *vkey = vk & 0xFF;
    }

}
DWORD QLuaScript::mapQtKey2Win(QString seq)
{
    DWORD vkey = 0;
    DWORD mod = 0;
    mapQtKey2Win(seq,&mod,&vkey);
    return QScriptHelper::makeHKId(mod,vkey);
}

QString QLuaScript::getUserFolder(bool amust)
{
    return Helper->getUserFolder(amust);
}

void QLuaScript::addStopHandler(lua_State* thread,QString func)
{
    if (thread==luaVM)
    {
       // lua_pushstring(luaVM,"addStopHandler must be called from thread.");
        //luaerror();
    }
    StopHandlers[func] = thread;
}

void QLuaScript::addWinHandler(lua_State* thread,QString func)
{
    if (thread==luaVM)
    {
        //lua_pushstring(luaVM,"addWinHandler must be called from thread.");
        //luaerror();
    }
    WinHandlers[func] = thread;
}

void QLuaScript::removeWinHandler(lua_State* thread,QString func)
{
    if (WinHandlers.count(func))
        WinHandlers.erase(func);
}

void QLuaScript::processStopHandlers()
{
    if (stoppingnow) return;
    stoppingnow = true;
    WinHandlers.clear();

    for (QStopHandlers::iterator i=StopHandlers.begin();i!=StopHandlers.end();i++)
    {
        QString func = i->first;
        lua_getglobal(luaVM,func.toLocal8Bit().data());
        int err = lua_pcall(luaVM,0,0,0);
        if (err)
        {
            //qDebug()<<"popping stops..."<<lua_gettop(luaVM);
            lua_pop(luaVM,1);
        }
        err = err + 1;
    }
    StopHandlers.clear();
}

void  QLuaScript::processWinHandlers(int num)
{
    for (QStopHandlers::iterator i=WinHandlers.begin();i!=WinHandlers.end();i++)
    {
        QString func = i->first;
        lua_getglobal(luaVM,func.toLocal8Bit().data());
        lua_pushnumber(luaVM,num);
        if (LUA_OK!=lua_pcall(luaVM,1,0,0))
            luaerror();
    }
}

DWORD QLuaScript::getIdByState(lua_State *L)
{
    if (Reverse.count(L))
      return Reverse[L];
    return STOP_HOTKEY;
}
