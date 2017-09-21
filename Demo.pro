#-------------------------------------------------
#
# Project created by QtCreator 2017-09-17T16:51:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    objloader.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    objloader.h \
    glwidget.h

FORMS    +=

RESOURCES += \
    shaders.qrc \
    obj.qrc
