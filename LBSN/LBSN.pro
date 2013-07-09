#-------------------------------------------------
#
# Project created by QtCreator 2013-07-09T09:50:22
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = LBSN
TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11
DEFINES += LBSN_LIBRARY

SOURCES += \
    foursquare.cpp

HEADERS +=\
        lbsn_global.h \
    foursquare.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
