#-------------------------------------------------
#
# Project created by QtCreator 2021-08-20T07:50:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = funcTableGenerator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingdialog.cpp \
    filecontrol.cpp \
    codegenerate.cpp \
    functionpointerdialog.cpp \
    structuredialog.cpp \
    elementdialog.cpp \
    fileprintform.cpp \
    record.cpp

HEADERS  += mainwindow.h \
    settingdialog.h \
    filecontrol.h \
    codegenerate.h \
    functionpointerdialog.h \
    structuredialog.h \
    elementdialog.h \
    fileprintform.h \
    record.h

FORMS    += mainwindow.ui \
    settingdialog.ui \
    functionpointerdialog.ui \
    structuredialog.ui \
    elementdialog.ui \
    fileprintform.ui
