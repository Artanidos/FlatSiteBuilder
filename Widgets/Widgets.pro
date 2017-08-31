#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T21:29:57
#
#-------------------------------------------------

QT       += widgets

TARGET = Widgets
TEMPLATE = lib

DEFINES += WIDGETS_LIBRARY

SOURCES += \
    flatbutton.cpp \
    imageselector.cpp

HEADERS +=\
    flatbutton.h \
    imageselector.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
