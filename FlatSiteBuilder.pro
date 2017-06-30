#-------------------------------------------------
#
#    Project created by QtCreator 2016-12-19T11:03:34
#
#-------------------------------------------------
#    Copyright (C) 2017 Olaf Japp
#    https://facebook.com/artanidos
#
#    This file is part of FlatSiteBuilder.
#
#    FlatSiteBuilder is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Foobar is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.

QT += core gui qml quickwidgets quickcontrols2 quick
QT += testlib 
QT += widgets

unix:PYTHON_VERSION=2.7
TARGET = FlatSiteBuilder
TEMPLATE = app
target.path += /bin
INSTALLS += target

SOURCES += main.cpp\
    mainwindow.cpp \
    generator.cpp \
    htmlhighlighter.cpp \
    expander.cpp \
    hyperlink.cpp \
    pages.cpp \
    posts.cpp

HEADERS  += mainwindow.h \
    generator.h \
    htmlhighlighter.h \
    expander.h \
    hyperlink.h \
    pages.h \
    posts.h

DISTFILES += \
    README.md \
    testsite/config.yaml \
    testsite/about.md \
    testsite/index.md \
    testsite/themes/default/layouts/default.html \
    testsite/themes/default/includes/footer.html \
    testsite/themes/default/includes/head.html \
    testsite/themes/default/includes/header.html \
    testsite/themes/default/includes/nav.html \
    testsite/themes/default/assets/css/style.css \
    dashboard.qml \
    PageModel.qml \
    qml/Dashboard.qml \
    qml/PageModel.qml \
    python/python.py

linux-g++ {
    LIBS += -L$$PWD/lib/
    LIBS += -lm
    LIBS += -ldl

    LIBS += -L$$PWD/dependencies/PythonQt3.1/lib/
    LIBS += -lPythonQt
    LIBS += -lPythonQt_QtAll

    LIBS += $$system(python$${PYTHON_VERSION}-config --libs)
    QMAKE_CXXFLAGS += $$system(python$${PYTHON_VERSION}-config --includes)


    INCLUDEPATH += $$PWD/dependencies/PythonQt3.1/include
    DEPENDPATH += $$PWD/dependencies/PythonQt3.1/include
}

RESOURCES += \
    flatsitebuilder.qrc
