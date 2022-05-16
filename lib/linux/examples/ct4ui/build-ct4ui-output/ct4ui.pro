#-------------------------------------------------
#
# Project created by QtCreator 2017-08-02T14:32:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ct4ui
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    waveformviewer.cpp \
    vitalsbox.cpp \
    batterybox.cpp \
    snrbox.cpp \
    pressuretargetbox.cpp \
    calibrationdialog.cpp \

HEADERS  += mainwindow.h \
    pulsewaveform.h \
    waveformviewer.h \
    vitalsbox.h \
    batterybox.h \
    snrbox.h \
    pressuretargetbox.h \
    calibrationdialog.h \

FORMS    += mainwindow.ui \
    calibrationdialog.ui

RESOURCES     = ct4ui.qrc

unix:!macx: LIBS += -L$$PWD/lib -lcaretaker
unix:!macx: LIBS += -L$$PWD/lib -lbluetooth

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib
