#-------------------------------------------------
#
# Project created by QtCreator 2011-09-30T05:05:20
#
#-------------------------------------------------

QT       += core gui xml network

TARGET = gcupd
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    ../Common/md5.cpp \
    ../Common/qautomessagebox.cpp

HEADERS  += dialog.h \
    ../Common/md5.h \
    ../Common/qautomessagebox.h

FORMS    += dialog.ui

RESOURCES += \
    upd_rsrs.qrc





