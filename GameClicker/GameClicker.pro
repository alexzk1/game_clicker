#-------------------------------------------------
#
# Project created by QtCreator 2011-11-04T16:18:38
#
#-------------------------------------------------

QT       += core gui network widgets
DEFINES  += QT_DISABLE_DEPRECATED_BEFORE=0x000000

TARGET = GameClicker
TEMPLATE = app

unix:!macx:QMAKE_CXXFLAGS += -march=native

QMAKE_CXXFLAGS +=  -std=c++14 -Wall -frtti -fexceptions -Werror=return-type -Werror=overloaded-virtual
QMAKE_CXXFLAGS +=  -Wctor-dtor-privacy -Werror=delete-non-virtual-dtor -Werror=strict-aliasing -fstrict-aliasing
CONFIG += c++14

#using new C++ libs for macos http://blog.michael.kuron-germany.de/2013/02/using-c11-on-mac-os-x-10-8/
#that may not work with C++14 though, Apple is slow
macx: QMAKE_CXXFLAGS += -stdlib=libc++
macx: QMAKE_LFLAGS += -lc++
macx: QMAKE_CXXFLAGS += -mmacosx-version-min=10.10
macx: QMAKE_MACOSX_DEPLOYMENT_TARGET=10.10

SOURCES += main.cpp\
        mainwindow.cpp \
    qluascript.cpp \
    qwindowtargeter.cpp \
    showhk.cpp \
    qfixgg.cpp \
    qgcbroadcaster.cpp

HEADERS  += mainwindow.h \
    qluascript.h \
    qwindowtargeter.h \
    Versions.h \
    showhk.h \
    qgcbroadcaster.h \
    qfixgg.h \
    fake_types.h

FORMS    += mainwindow.ui \
    qwindowtargeter.ui \
    showhk.ui

include ($$PWD/lua/lua_vm.pri)

win32:{
  DEFINES+= "DETOURS_X86=\"1\""
  DEFINES+= USE_GG_FIX
  SOURCES += disasm.cpp
  LIBS += -lgdi32
}

RESOURCES += \
    GCRes.qrc

TRANSLATIONS = gc_ru.ts


