#-------------------------------------------------
#
# Project created by QtCreator 2016-10-24T15:26:16
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyPlayer_
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mybutton.cpp \
    myslider.cpp \
    mytablewidget.cpp \
    mymusicitem.cpp \
    rightmenubutton.cpp \
    rightkeymenu.cpp \
    volumeslider.cpp \
    cube.cpp \
    playmodebutton.cpp \
    settings.cpp \
    mylrc.cpp \
    video.cpp \
    videowidget.cpp

HEADERS  += widget.h \
    mybutton.h \
    myslider.h \
    mytablewidget.h \
    mymusicitem.h \
    rightmenubutton.h \
    rightkeymenu.h \
    volumeslider.h \
    cube.h \
    playmodebutton.h \
    settings.h \
    mylrc.h \
    video.h \
    videowidget.h

RESOURCES += \
    images.qrc

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/qxtglobal.h \
           $$PWD/qxtglobalshortcut.h \
           $$PWD/qxtglobalshortcut_p.h
SOURCES += $$PWD/qxtglobalshortcut.cpp
win32{
    SOURCES += $$PWD/qxtglobalshortcut_win.cpp
    LIBS += -luser32
}
unix:SOURCES += $$PWD/qxtglobalshortcut_x11.cpp
mac:SOURCES += $$PWD/qxtglobalshortcut_mac.cpp

DISTFILES += \
    ico.rc
RC_FILE = \
    ico.rc
