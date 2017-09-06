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

QT       += widgets

TARGET = Widgets
TEMPLATE = lib

DEFINES += WIDGETS_LIBRARY

SOURCES += \
    flatbutton.cpp \
    imageselector.cpp \
    tablecheckbox.cpp

HEADERS +=\
    flatbutton.h \
    imageselector.h \
    tablecheckbox.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES +=
