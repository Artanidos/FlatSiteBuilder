QT           += widgets xml gui core
TEMPLATE      = lib
CONFIG       += plugin
TARGET		  = ImageEditor
INCLUDEPATH  += ../../
SOURCES		 += imageeditor.cpp
HEADERS		 += imageeditor.h \
    ../../interfaces.h
DESTDIR       = ~/FlatSiteBuilder/plugins

unix:!macx: LIBS += -L$$OUT_PWD/../../Widgets/ -lWidgets

INCLUDEPATH += $$PWD/../../Widgets
DEPENDPATH += $$PWD/../../Widgets

RESOURCES += \
    images.qrc
