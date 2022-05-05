TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

DISTFILES += \
    README.md

HEADERS += \
    ConcurrentBlockingQueue.h \
    ConcurrentQueue.h
