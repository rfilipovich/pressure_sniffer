#-------------------------------------------------
#
# Project created by QtCreator 2025-02-09T20:31:48
#
#-------------------------------------------------

QT       += core gui

CONFIG   += console
CONFIG   += debug

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pressure_sniffer
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    waitwindow.cpp \
    freq_change_form.cpp \
    startdialog.cpp

HEADERS += \
        mainwindow.h \
    waitwindow.h \
    freq_change_form.h \
    startdialog.h

FORMS += \
        mainwindow.ui \
    waitwindow.ui \
    freq_change_form.ui \
    startdialog.ui

RESOURCES += \
    presssure_sniffer.qrc

target.path = /oem/
INSTALLS += target



CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS ~= s/-O[0123s]//g -funwind-tables
    QMAKE_CFLAGS ~= s/-O[0123s]//g -funwind-tables
    QMAKE_LFLAGS ~= s/-O[0123s]//g -funwind-tables
    QMAKE_CXXFLAGS += -g -O0 -DDEBUG -funwind-tables
    QMAKE_CFLAGS += -g -O0 -DDEBUG - funwind-tables
    QMAKE_LFLAGS += -g -O0 -DDEBUG -funwind-tables

#    SOURCES  += LeakTracer.cpp
}

include(common/common.pri)
include(linux/linux.pri)
include(sysconfig/SysConfig.pri)
