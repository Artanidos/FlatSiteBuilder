QT += widgets testlib xml core gui
SOURCES = testgenerator.cpp

DISTFILES += \
    pages/test.xml

unix:!macx: LIBS += -L$$OUT_PWD/../Widgets/ -lWidgets

INCLUDEPATH += $$PWD/../Widgets
DEPENDPATH += $$PWD/../Widgets
