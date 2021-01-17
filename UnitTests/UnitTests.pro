QT += widgets testlib

TARGET = tst_statisticstest.cc
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_statisticstest.cc \
    ../Game/statistics.cpp

HEADERS += ../Game/statistics.hh

INCLUDEPATH += \
            ../../Game/

DEPENDPATH += ../Game/
