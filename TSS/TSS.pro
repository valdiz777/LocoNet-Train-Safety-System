#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T23:23:52
#
#-------------------------------------------------

QT       += core gui sql network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TSS
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        locopacket.cpp \
    locobyte.cpp \
    locotrain.cpp \
    locoblock.cpp \
    locoserial.cpp \
    locosql.cpp \
    locoudp.cpp \
    section.cpp \
    trainmonitor.cpp \
    enginepath.cpp \
    log.cpp

HEADERS  += mainwindow.h \
        locopacket.h \
    locobyte.h \
    locotrain.h \
    locoblock.h \
    locoserial.h \
    locosql.h \
    locoudp.h \
    section.h \
    enginepath.h \
    trainmonitor.h \
    log.h

FORMS    += mainwindow.ui

DISTFILES += \
    Sections.txt
