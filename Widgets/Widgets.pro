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
#    FlatSiteBuilder is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.

QT       += widgets testlib

TARGET = Widgets
TEMPLATE = lib

DEFINES += WIDGETS_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    flatbutton.cpp \
    imageselector.cpp \
    tablecellbuttons.cpp \
    undoableeditor.cpp \
    xmlhighlighter.cpp \
    site.cpp \
    generator.cpp \
    content.cpp \
    menu.cpp \
    menuitem.cpp \
    plugins.cpp \
    sectionpropertyeditor.cpp \
    rowpropertyeditor.cpp \
    column.cpp

HEADERS +=\
    flatbutton.h \
    imageselector.h \
    widgets_global.h \
    tablecellbuttons.h \
    undoableeditor.h \
    animateableeditor.h \
    xmlhighlighter.h \
    site.h \
    generator.h \
    content.h \
    interfaces.h \
    menu.h \
    menuitem.h \
    plugins.h \
    sectionpropertyeditor.h \
    rowpropertyeditor.h \
    column.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
