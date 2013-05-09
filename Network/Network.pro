#-------------------------------------------------
#
# Project created by QtCreator 2013-04-25T15:33:30
#
#-------------------------------------------------

QT       -= gui

TARGET = Network
TEMPLATE = lib
CONFIG += lib_bundle
DEFINES += NETWORK_LIBRARY

mac:QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++
else:QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    network_measurements.cpp \
    network_generator.cpp \
    network_apps.cpp \
    graph_io.cpp

HEADERS += network.h\
        network_global.h \
    network.hpp \
    graph_io.hpp \
    graph.hpp \
    network_measurements.hpp \
    network_generator.hpp \
    network_apps.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Math/release/ -lMath
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Math/debug/ -lMath
else:mac: LIBS += -F$$OUT_PWD/../Math/ -framework Math
else:unix: LIBS += -L$$OUT_PWD/../Math/ -lMath

INCLUDEPATH += $$PWD/../Math
DEPENDPATH += $$PWD/../Math
