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

QT += core gui
QT += testlib
QT += widgets

TARGET = FlatSiteBuilder
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    generator.cpp \
    expander.cpp \
    hyperlink.cpp \
    site.cpp \
    column.cpp \
    dashboard.cpp \
    content.cpp \
    contenteditor.cpp \
    contentlist.cpp \
    sectioneditor.cpp \
    pageeditor.cpp \
    roweditor.cpp \
    elementeditor.cpp \
    columneditor.cpp \
    widgetmimedata.cpp \
    dropzone.cpp \
    columnsdialog.cpp \
    moduldialog.cpp \
    menu.cpp \
    menuitem.cpp \
    rowpropertyeditor.cpp \
    sectionpropertyeditor.cpp \
    sitewizard.cpp \
    commands.cpp \
    plugins.cpp \
    menulist.cpp \
    menueditor.cpp \
    themechooser.cpp \
    sitesettingseditor.cpp \
    installdialog.cpp

HEADERS  += mainwindow.h \
    generator.h \
    expander.h \
    hyperlink.h \
    site.h \
    column.h \
    dashboard.h \
    content.h \
    contenteditor.h \
    contentlist.h \
    sectioneditor.h \
    pageeditor.h \
    roweditor.h \
    elementeditor.h \
    columneditor.h \
    widgetmimedata.h \
    dropzone.h \
    columnsdialog.h \
    moduldialog.h \
    menu.h \
    menuitem.h \
    interfaces.h \
    rowpropertyeditor.h \
    sectionpropertyeditor.h \
    sitewizard.h \
    commands.h \
    plugins.h \
    menulist.h \
    menueditor.h \
    themechooser.h \
    sitesettingseditor.h \
    installdialog.h

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
    buildAppImage.sh \
    themes/default/includes/postheader.html \
    themes/default/includes/postnav.html \
    themes/default/assets/css/bootstrap.min.css \
    themes/default/assets/css/bootstrap-theme.min.css \
    themes/default/assets/css/theme.css \
    themes/default/assets/fonts/glyphicons-halflings-regular.eot \
    themes/default/assets/fonts/glyphicons-halflings-regular.woff \
    themes/default/assets/fonts/glyphicons-halflings-regular.woff2 \
    themes/default/assets/fonts/glyphicons-halflings-regular.ttf \
    themes/default/assets/fonts/glyphicons-halflings-regular.svg \
    themes/default/assets/js/bootstrap.min.js \
    themes/default/assets/js/npm.js \
    testsite/includes/footercontent.html \
    web/pages/index.xml \
    web/pages/notimpl.xml \
    web/pages/publish.xml \
    web/Site.xml \
    web/assets/images/olaf winter.jpg \
    web/assets/images/olaf.jpg \
    web/assets/images/Columns.png \
    web/assets/images/Dashboard.png \
    web/assets/images/Editor.png \
    web/assets/images/Layout.png \
    web/assets/images/logo.png \
    web/assets/images/logo_dark.png \
    web/assets/images/Modules.png \
    web/assets/images/tagcloud.png \
    Screenshot.png \
    web/assets/css/style.css \
    web/includes/footercontent.html \
    web/layouts/simple.html \
    changelog.txt \
    testsite/Menus.xml \
    themes/default/assets/images/favicon.ico \
    themes/default/assets/css/jumbotron.css \
    themes/default/includes/scripts.html \
    themes/himu/assets/js/bootstrap.min.js \
    themes/himu/assets/js/html5shiv.js \
    themes/himu/assets/js/jquery.isotope.min.js \
    themes/himu/assets/js/jquery.js \
    themes/himu/assets/js/jquery.parallax.js \
    themes/himu/assets/js/jquery.prettyPhoto.js \
    themes/himu/assets/js/main.js \
    themes/himu/assets/js/respond.min.js \
    themes/himu/assets/js/smoothscroll.js \
    themes/himu/assets/fonts/fontawesome-webfont.woff \
    themes/himu/assets/fonts/glyphicons-halflings-regular.woff \
    themes/himu/assets/fonts/FontAwesome.otf \
    themes/himu/assets/images/prettyPhoto/dark_rounded/default_thumbnail.gif \
    themes/himu/assets/images/prettyPhoto/dark_rounded/loader.gif \
    themes/himu/assets/images/prettyPhoto/dark_square/default_thumbnail.gif \
    themes/himu/assets/images/prettyPhoto/dark_square/loader.gif \
    themes/himu/assets/images/prettyPhoto/default/loader.gif \
    themes/himu/assets/images/prettyPhoto/facebook/default_thumbnail.gif \
    themes/himu/assets/images/prettyPhoto/facebook/loader.gif \
    themes/himu/assets/images/prettyPhoto/light_rounded/default_thumbnail.gif \
    themes/himu/assets/images/prettyPhoto/light_rounded/loader.gif \
    themes/himu/assets/images/prettyPhoto/light_square/default_thumbnail.gif \
    themes/himu/assets/images/prettyPhoto/light_square/loader.gif \
    themes/himu/assets/images/about-us/about.jpg \
    themes/himu/assets/images/about-us/community.jpg \
    themes/himu/assets/images/about-us/mission.jpg \
    themes/himu/assets/images/blog/1.jpg \
    themes/himu/assets/images/blog/2.jpg \
    themes/himu/assets/images/blog/3.jpg \
    themes/himu/assets/images/clients/client-bg.jpg \
    themes/himu/assets/images/clients/client1.jpg \
    themes/himu/assets/images/clients/client2.jpg \
    themes/himu/assets/images/clients/client3.jpg \
    themes/himu/assets/images/our-team/member1.jpg \
    themes/himu/assets/images/our-team/member2.jpg \
    themes/himu/assets/images/our-team/member3.jpg \
    themes/himu/assets/images/our-team/member4.jpg \
    themes/himu/assets/images/portfolio/1.jpg \
    themes/himu/assets/images/portfolio/10.jpg \
    themes/himu/assets/images/portfolio/11.jpg \
    themes/himu/assets/images/portfolio/12.jpg \
    themes/himu/assets/images/portfolio/2.jpg \
    themes/himu/assets/images/portfolio/3.jpg \
    themes/himu/assets/images/portfolio/4.jpg \
    themes/himu/assets/images/portfolio/5.jpg \
    themes/himu/assets/images/portfolio/6.jpg \
    themes/himu/assets/images/portfolio/7.jpg \
    themes/himu/assets/images/portfolio/8.jpg \
    themes/himu/assets/images/portfolio/9.jpg \
    themes/himu/assets/images/portfolio/big-item.jpg \
    themes/himu/assets/images/portfolio/big-item2.jpg \
    themes/himu/assets/images/portfolio/big-item3.jpg \
    themes/himu/assets/images/portfolio/big-item4.jpg \
    themes/himu/assets/images/services/service-bg.jpg \
    themes/himu/assets/images/slider/slide1.jpg \
    themes/himu/assets/images/slider/slide2.jpg \
    themes/himu/assets/images/slider/slide3.jpg \
    themes/default/sample.png \
    themes/himu/assets/images/ico/apple-touch-icon-114-precomposed.png \
    themes/himu/assets/images/ico/apple-touch-icon-144-precomposed.png \
    themes/himu/assets/images/ico/apple-touch-icon-57-precomposed.png \
    themes/himu/assets/images/ico/apple-touch-icon-72-precomposed.png \
    themes/himu/assets/images/ico/favicon.png \
    themes/himu/assets/images/prettyPhoto/dark_rounded/btnNext.png \
    themes/himu/assets/images/prettyPhoto/dark_rounded/btnPrevious.png \
    themes/himu/assets/images/prettyPhoto/dark_rounded/contentPattern.png \
    themes/himu/assets/images/prettyPhoto/dark_rounded/sprite.png \
    themes/himu/assets/images/prettyPhoto/dark_square/btnNext.png \
    themes/himu/assets/images/prettyPhoto/dark_square/btnPrevious.png \
    themes/himu/assets/images/prettyPhoto/dark_square/contentPattern.png \
    themes/himu/assets/images/prettyPhoto/dark_square/sprite.png \
    themes/himu/assets/images/prettyPhoto/default/default_thumb.png \
    themes/himu/assets/images/prettyPhoto/default/sprite.png \
    themes/himu/assets/images/prettyPhoto/default/sprite_next.png \
    themes/himu/assets/images/prettyPhoto/default/sprite_prev.png \
    themes/himu/assets/images/prettyPhoto/default/sprite_x.png \
    themes/himu/assets/images/prettyPhoto/default/sprite_y.png \
    themes/himu/assets/images/prettyPhoto/facebook/btnNext.png \
    themes/himu/assets/images/prettyPhoto/facebook/btnPrevious.png \
    themes/himu/assets/images/prettyPhoto/facebook/contentPatternBottom.png \
    themes/himu/assets/images/prettyPhoto/facebook/contentPatternLeft.png \
    themes/himu/assets/images/prettyPhoto/facebook/contentPatternRight.png \
    themes/himu/assets/images/prettyPhoto/facebook/contentPatternTop.png \
    themes/himu/assets/images/prettyPhoto/facebook/sprite.png \
    themes/himu/assets/images/prettyPhoto/light_rounded/btnNext.png \
    themes/himu/assets/images/prettyPhoto/light_rounded/btnPrevious.png \
    themes/himu/assets/images/prettyPhoto/light_rounded/sprite.png \
    themes/himu/assets/images/prettyPhoto/light_square/btnNext.png \
    themes/himu/assets/images/prettyPhoto/light_square/btnPrevious.png \
    themes/himu/assets/images/prettyPhoto/light_square/sprite.png \
    themes/himu/assets/images/home-pattern.png \
    themes/himu/assets/images/logo.png \
    themes/himu/assets/images/pattern.png \
    themes/himu/sample.png \
    themes/himu/assets/fonts/fontawesome-webfont.svg \
    themes/himu/assets/fonts/glyphicons-halflings-regular.svg \
    themes/himu/assets/css/animate.css \
    themes/himu/assets/css/bootstrap.min.css \
    themes/himu/assets/css/font-awesome.min.css \
    themes/himu/assets/css/jquery.sidr.dark.css \
    themes/himu/assets/css/main.css \
    themes/himu/assets/css/prettyPhoto.css \
    themes/himu/assets/css/reset.css \
    themes/himu/assets/css/responsive.css \
    themes/default/sample.html \
    themes/himu/includes/nav.html \
    themes/himu/layouts/default.html \
    themes/himu/sample.html \
    default.desktop \
    default.svg \
    AppRun

linux-g++ {
    LIBS += -L$$PWD/lib/
    LIBS += -lm
    LIBS += -ldl
}

RESOURCES += \
    flatsitebuilder.qrc

LIBS += -L$$OUT_PWD/Widgets/ -lWidgets
INCLUDEPATH += $$PWD/Widgets
DEPENDPATH += $$PWD/Widgets
