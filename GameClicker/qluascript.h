#ifndef QLUASCRIPT_H
#define QLUASCRIPT_H
#include <QObject>
#include <map>
#include <list>
#include <algorithm>
#include <queue>
#include "fake_types.h"
#include "lua/luasrc/customlua.h"
#include "qfixgg.h"

class QScriptHelper
{
private:


protected:
    typedef std::list<DWORD> QHKIds;
    struct QHKText
    {
        QString Func;
        QString Text;
    };
    typedef std::map<QString,QHKText> QHKTexts;
    virtual void hkTextsChanged() = 0;


    QHKIds Ids;
    QHKTexts Texts;

    DWORD addHKId(DWORD id)
    {
        Ids.push_back(id);
        return id;
    }
    void unsetAllP(HWND win)
    {
        if (Ids.empty())
            return;
        DWORD id;
        foreach(id,Ids)
            UnregisterHotKey(win,id);
        Ids.clear();
        clearHKTexts();
    }
    void clearHKTexts()
    {
        Texts.clear();
        hkTextsChanged();
    }

public:
    bool isIdRegistered(DWORD id)
    {
        QHKIds::iterator i = std::find(Ids.begin(),Ids.end(),id);
        return i!=Ids.end();
    }

public:
    virtual HWND  getWindow(unsigned long id)=0;
    virtual int   getWinCount()=0;
    virtual COLORREF getColor(HWND win, int x, int y) = 0;
    virtual DWORD setHotKey(DWORD mod, DWORD vkey) = 0;
    virtual void  emulateHotKey(DWORD mod, DWORD vkey) = 0;
    virtual QString getUserFolder(bool request=false) = 0;
    void          addHKText(QString Seq,QString Func,QString Text)
    {
        QHKText t;
        t.Func = Func;
        t.Text = Text;
        Texts[Seq] = t;
        hkTextsChanged();
    }

    virtual void  unsetHKAll() = 0;
    static  DWORD makeHKId(DWORD mod, DWORD vkey)
    {
        return (mod << 8) | vkey;
    }
};

//#define STOP_HOTKEY QLuaScript::mapQtKey2Win("ctrl+alt+pause")
#define STOP_HOTKEY 0xFFFFFFFF

class QLuaScript:public QObject
{
    Q_OBJECT
public:
    typedef std::map<lua_State *,QLuaScript*> QThisFinder;
    typedef std::map<lua_State *,DWORD> QReverseSrch;
    typedef std::map<QString,QFixGG> QGuards;
    typedef enum {THSERROR,THSDONE,THSSUSPENDED,THSWAITYET} QThreadState;
private:
    struct QHotSignal {
        QString    luaFunc;
        lua_State* thread;
        quint32    suspendedTill;
        quint32    hcounter; //hotkeys pressed counter
        bool       critical;
        int        criticalcounter;
        bool       entryrun;
        QHotSignal()
        {
            luaFunc  = "";
            clear();
        }
        void clear()
        {
            suspendedTill = 0xFFFFFFFF;
            hcounter = 0;
            thread   = NULL;
            critical = false;
            entryrun = true;
            criticalcounter = 0;
        }
    };
    typedef std::map<DWORD, QHotSignal>QCoMap;
    typedef std::deque<DWORD> QYielded;
    typedef std::map<QString,lua_State*> QStopHandlers;
public:
    explicit QLuaScript(QScriptHelper *helper, QObject *parent = 0);
    virtual ~QLuaScript();

    static QLuaScript* getThis(lua_State*);
    lua_State*         getVM();

    bool               runScriptFile(QString Script);
    HWND               getWindow(unsigned long num);
    int                getWinCount();
    const QFixGG*      getGuard(QString Func, QString Library);
    bool               setHotKey(QString keyStr, QString func,QString text="");
    quint32            getHKCounter(QString func);
    static void        mapQtKey2Win(QString seq,DWORD *mod,DWORD *vkey);
    static DWORD       mapQtKey2Win(QString seq);
    QWidget*           getOwner();
    QString            getUserFolder(bool amust);
    void               addStopHandler(lua_State* thread,QString func);
    void               addWinHandler(lua_State* thread,QString func);
    void               removeWinHandler(lua_State* thread,QString func);

    void               setCritical(lua_State* L, bool enable);
    void               emulateHotKey(QString hk);
    void               stopNow();
    bool               shouldYield(lua_State* L);
protected:
    void        closeVM(bool dummy);
    bool        stoppingnow;
    static void addThis(lua_State*, QLuaScript*);
    static void delThis(lua_State*);
    void        luaerror();
    void        luaerror(lua_State* L);
    void        luaSetupGlobals();
    void        clearHotSignals();
    bool        runScript(QString Script,bool is_filename);
    void        processStopHandlers();
    void        processWinHandlers(int num);
    DWORD       getIdByState(lua_State *L);
private:
    static QThisFinder thisFinder;
    QGuards Guards;
    lua_State          *luaVM; //vm of the script
    QCoMap Functions;
    QYielded Yielded;
    QScriptHelper* Helper;
    QStopHandlers StopHandlers;
    QStopHandlers WinHandlers;
    QReverseSrch Reverse;
    QString rerun_fn;
    bool    rerun_is;


    bool createThread(DWORD id);
    void destroyThread(DWORD id);
    QThreadState runThread(DWORD id);
    void hotUserCntr(DWORD id);
private slots:
    void threadsLoop();
    void reRunRun();
public slots:
    void hotKeyByUser(DWORD id);
signals:
    void started();
    void stopped();
};

#endif // QLUASCRIPT_H
