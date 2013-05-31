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
    qnetwork.cpp \
    distance_computation.cpp \
    lambda_computation.cpp \
    dialog_enhance_sync.cpp

HEADERS  += main_window.h \
    qnetwork.hpp \
    distance_computation.h \
    lambda_computation.h \
    dialog_enhance_sync.h

FORMS    += main_window.ui \
    dialog_enhance_sync.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Network/release/ -lNetwork
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Network/debug/ -lNetwork
else:mac: LIBS += -F$$OUT_PWD/../Network/ -framework Network
else:unix: LIBS += -L$$OUT_PWD/../Network/ -lNetwork

INCLUDEPATH += $$PWD/../Network
DEPENDPATH += $$PWD/../Network

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Math/release/ -lMath
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Math/debug/ -lMath
else:mac: LIBS += -F$$OUT_PWD/../Math/ -framework Math
else:unix: LIBS += -L$$OUT_PWD/../Math/ -lMath

INCLUDEPATH += $$PWD/../Math
DEPENDPATH += $$PWD/../Math
