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

QT += widgets testlib xml network
SOURCES = testgenerator.cpp \
    ../generator.cpp \
    ../plugins.cpp \
    ../sectionpropertyeditor.cpp \
    ../rowpropertyeditor.cpp \
    ../column.cpp \
    ../site.cpp \
    ../content.cpp \
    ../menu.cpp \
    ../menuitem.cpp
DEFINES += TEST
DISTFILES += \
    pages/test.xml \
    themes/testSiteTheme/includes/nav.html \
    themes/testSiteTheme/layouts/testPageLayout.html

HEADERS += \
    ../generator.h \
    ../plugins.h \
    ../column.h \
    ../rowpropertyeditor.h \
    ../sectionpropertyeditor.h \
    ../interfaces.h \
    ../site.h \
    ../content.h \
    ../menu.h \
    ../menuitem.h


LIBS += -L$$OUT_PWD/../Widgets/ -lWidgets
INCLUDEPATH += $$PWD/../Widgets
DEPENDPATH += $$PWD/../Widgets
