#-------------------------------------------------
#
# Project created by QtCreator 2013-04-25T15:30:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = App
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        main_window.cpp \
    qnetwork.cpp

HEADERS  += main_window.h \
    qnetwork.hpp

FORMS    += main_window.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Network/release/ -lNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Network/debug/ -lNetwork
else:unix: LIBS += -L$$OUT_PWD/../Network/ -lNetwork

INCLUDEPATH += $$PWD/../Network
DEPENDPATH += $$PWD/../Network

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Math/release/ -lMath
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Math/debug/ -lMath
else:unix: LIBS += -L$$OUT_PWD/../Math/ -lMath

INCLUDEPATH += $$PWD/../Math
DEPENDPATH += $$PWD/../Math
