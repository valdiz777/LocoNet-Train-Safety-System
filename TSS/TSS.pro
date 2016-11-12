#-------------------------------------------------
#
# Project created by QtCreator 2016-10-14T23:23:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(3rdparty/qextserialport/src/qextserialport.pri)
release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

TARGET = TSS
CONFIG += c++11
TEMPLATE = app
SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/locopacket.cpp \
    src/locobyte.cpp \
    src/locotrain.cpp \
    src/locoblock.cpp \
    src/locoserial.cpp \
    src/section.cpp \
    src/trainmonitor.cpp \

HEADERS  += src/mainwindow.h \
        src/locopacket.h \
    src/locobyte.h \
    src/locotrain.h \
    src/locoblock.h \
    src/section.h \
    src/trainmonitor.h \
    src/locoserial.h

FORMS    += src/mainwindow.ui

RESOURCES += \
    tss.qrc
