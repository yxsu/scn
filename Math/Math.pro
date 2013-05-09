#-------------------------------------------------
#
# Project created by QtCreator 2013-04-25T15:47:54
#
#-------------------------------------------------

QT       -= gui

TARGET = Math
TEMPLATE = lib
CONFIG += lib_bundle
DEFINES += MATH_LIBRARY

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    wavelet.cpp \
    sym_matrix.cpp \
    sqr_matrix.cpp \
    matrix.cpp \
    dwt_data.cpp

HEADERS += math.h\
        math_global.h \
    wavelet.hpp \
    sym_matrix.hpp \
    sqr_matrix.hpp \
    matrix.hpp \
    fortran2c.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
