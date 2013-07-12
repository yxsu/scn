#-------------------------------------------------
#
# Project created by QtCreator 2013-07-12T11:08:01
#
#-------------------------------------------------

QT       -= gui

TARGET = Crawler
TEMPLATE = lib
QMAKE_CXXFLAGS += -std=c++11

DEFINES += CRAWLER_LIBRARY

SOURCES += crawler.cpp \
    twitter_crawler.cpp

HEADERS += crawler.hpp\
        crawler_global.h \
    twitter_crawler.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
