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
    foursquare.hpp \

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Network/release/ -lNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Network/debug/ -lNetwork
else:mac: LIBS += -F$$OUT_PWD/../Network/ -framework Network
else:unix: LIBS += -L$$OUT_PWD/../Network/ -lNetwork

INCLUDEPATH += $$PWD/../Network
DEPENDPATH += $$PWD/../Network
