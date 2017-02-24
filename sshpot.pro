TARGET = sshpot
TEMPLATE = app
CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console c++11

LIBS += -lssh

HEADERS += \
    auth.h \
    config.h

SOURCES += \
    auth.cpp \
    main.cpp

