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

QT += core gui script
QT += testlib 
QT += widgets xml

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
    site.cpp \
    section.cpp \
    row.cpp \
    column.cpp \
    text.cpp \
    element.cpp \
    image.cpp \
    dashboard.cpp \
    content.cpp \
    contenteditor.cpp \
    contentlist.cpp \
    tablecheckbox.cpp \
    sectioneditor.cpp \
    pageeditor.cpp \
    roweditor.cpp \
    flatbutton.cpp \
    elementeditor.cpp \
    columneditor.cpp \
    widgetmimedata.cpp \
    dropzone.cpp \
    columnsdialog.cpp \
    moduldialog.cpp \
    textdialog.cpp \
    texteditor.cpp

HEADERS  += mainwindow.h \
    generator.h \
    htmlhighlighter.h \
    expander.h \
    hyperlink.h \
    site.h \
    section.h \
    row.h \
    column.h \
    text.h \
    element.h \
    image.h \
    dashboard.h \
    content.h \
    contenteditor.h \
    contentlist.h \
    tablecheckbox.h \
    sectioneditor.h \
    pageeditor.h \
    roweditor.h \
    flatbutton.h \
    elementeditor.h \
    columneditor.h \
    widgetmimedata.h \
    dropzone.h \
    columnsdialog.h \
    moduldialog.h \
    textdialog.h \
    texteditor.h

DISTFILES += \
    README.md \
    themes/default/layouts/default.html \
    themes/default/includes/footer.html \
    themes/default/includes/head.html \
    themes/default/includes/header.html \
    themes/default/includes/nav.html \
    themes/default/assets/css/style.css \
    python/python.py \
    testsite/Site.xml \
    testsite/pages/index.html \
    testsite/posts/hello.html \
    testsite/pages/about.html \
    testsite/pages/news.html \
    themes/default/layouts/post.html \
    themes/default/includes/posthead.html \
    test.js \
    testsite/pages/index.xml

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
