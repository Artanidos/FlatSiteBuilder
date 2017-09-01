QT           += widgets xml gui core
TEMPLATE      = lib
CONFIG       += plugin
TARGET		  = SampleEditor
INCLUDEPATH  += ../../
SOURCES		 += sampleeditor.cpp
HEADERS		 += sampleeditor.h \
    ../../interfaces.h
DESTDIR       = ~/FlatSiteBuilder/plugins

unix:!macx: LIBS += -L$$OUT_PWD/../../Widgets/ -lWidgets

INCLUDEPATH += $$PWD/../../Widgets
DEPENDPATH += $$PWD/../../Widgets

RESOURCES += \
    images.qrc
