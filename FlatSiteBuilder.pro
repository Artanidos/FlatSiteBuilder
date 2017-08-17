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
QT += testlib network
QT += widgets xml webenginewidgets

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
    texteditor.cpp \
    animationlabel.cpp \
    imageeditor.cpp \
    abstracteditor.cpp \
    imageselector.cpp \
    slidereditor.cpp \
    slider.cpp \
    menu.cpp \
    menuitem.cpp \
    rowpropertyeditor.cpp \
    sectionpropertyeditor.cpp \
    sitewizard.cpp

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
    texteditor.h \
    animationlabel.h \
    imageeditor.h \
    abstracteditor.h \
    imageselector.h \
    slidereditor.h \
    slider.h \
    menu.h \
    menuitem.h \
    rowpropertyeditor.h \
    sectionpropertyeditor.h \
    sitewizard.h

DISTFILES += \
    README.md \
    themes/default/layouts/default.html \
    themes/default/includes/footer.html \
    themes/default/includes/head.html \
    themes/default/includes/nav.html \
    themes/default/assets/css/style.css \
    testsite/Site.xml \
    themes/default/layouts/post.html \
    testsite/pages/index.xml \
    testsite/posts/hello.xml \
    testsite/pages/about.xml \
    testsite/pages/news.xml \
    todo.txt \
    build.txt \
    buildAppImage.sh

linux-g++ {
    LIBS += -L$$PWD/lib/
    LIBS += -lm
    LIBS += -ldl
}

RESOURCES += \
    flatsitebuilder.qrc
