#-------------------------------------------------
#
# Project created by QtCreator 2018-10-22T09:46:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BuckmarkTimeScale
TEMPLATE = app


SOURCES += main.cpp\
    timescale.cpp \
    timestamp.cpp \
    generatorthread_p.cpp \
    bookmarkmodel.cpp \
    bookmark.cpp \
    bookmarkview.cpp \
    bookmarkviewwidget.cpp \
    generatordialog.cpp

HEADERS  += \
    timescale.h \
    timestamp.h \
    generatorthread_p.h \
    bookmarkmodel.h \
    bookmark.h \
    bookmarkview.h \
    bookmarkviewwidget.h \
    generatordialog.h

FORMS    += \
    bookmarkview.ui \
    generatordialog.ui

RESOURCES += \
    images.qrc
