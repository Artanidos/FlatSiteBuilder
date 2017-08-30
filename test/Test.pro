QT += widgets testlib xml core gui
SOURCES = testgenerator.cpp \
    ../generator.cpp \
    ../site.cpp \
    ../section.cpp \
    ../row.cpp \
    ../column.cpp \
    ../image.cpp \
    ../slider.cpp \
    ../text.cpp \
    ../element.cpp \
    ../content.cpp \
    ../menu.cpp \
    ../menuitem.cpp

HEADERS += \
    ../element.h \
    ../column.h \
    ../generator.h \
    ../image.h \
    ../row.h \
    ../section.h \
    ../site.h \
    ../text.h \
    ../content.h \
    ../menu.h \
    ../menuitem.h

DISTFILES += \
    pages/test.xml
