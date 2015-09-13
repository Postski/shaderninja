#-------------------------------------------------
#
# Project created by QtCreator 2015-07-28T21:58:37
#
#-------------------------------------------------

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

TARGET = ShaderNinja
TEMPLATE = app

CONFIG += windows

CONFIG += c++11
CONFIG += c++14

LIBS += opengl32.lib

SOURCES += main.cpp \
    mainwindow.cpp \
    glwidget.cpp \
    dialogabout.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    dialogabout.h

RESOURCES += \
    defaultshaders.qrc \
    icons.qrc

FORMS += \
    mainwindow.ui \
    dialogabout.ui

RC_FILE += \
    sncommon.rc
