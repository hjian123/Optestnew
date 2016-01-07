#-------------------------------------------------
#
# Project created by QtCreator 2016-01-05T12:10:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opTestnew
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    canprotcl.cpp \
    cantest.cpp

HEADERS  += widget.h \
    canprotcl.h \
    cantest.h

FORMS    += widget.ui

DISTFILES += \
    img/cubemap_negx.jpg

RESOURCES += \
    img.qrc
