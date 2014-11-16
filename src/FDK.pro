#-------------------------------------------------
#
# Project created by QtCreator 2014-11-12T12:26:26
#
#-------------------------------------------------

QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FDK
TEMPLATE = app

INCLUDEPATH += "/usr/local/include"

SOURCES += main.cpp\
        fdkmainwindow.cpp \
    FDK/fdk.cpp \
    FDK/atom.cpp \
    FDK/brain/brain.cpp \
    FDK/brain/column.cpp \
    FDK/brain/cell.cpp \
    FDK/brain/synapse.cpp \
    FDK/brain/region.cpp \
    FDK/brain/segment.cpp \
    FDK/connection/connection.cpp \
    FDK/connection/sensor.cpp \
    sensorwidget.cpp \
    graphwidget.cpp \
    FDK/classifier/classifier.cpp \
    FDK/encoder/encoder.cpp \
    FDK/encoder/encoderinterface.cpp \
    FDK/encoder/sparsecharencoder.cpp \
    FDK/timevector.cpp

HEADERS  += fdkmainwindow.h \
    FDK/fdk.h \
    FDK/atom.h \
    FDK/brain/brain.h \
    FDK/brain/column.h \
    FDK/brain/cell.h \
    FDK/brain/synapse.h \
    FDK/brain/region.h \
    FDK/brain/segment.h \
    FDK/connection/connection.h \
    FDK/connection/sensor.h \
    FDK/fdkstructures.h \
    sensorwidget.h \
    graphwidget.h \
    FDK/classifier/classifier.h \
    FDK/encoder/encoder.h \
    FDK/encoder/encoderinterface.h \
    FDK/encoder/sparsecharencoder.h \
    FDK/timevector.h

FORMS    += fdkmainwindow.ui

QMAKE_CXXFLAGS += -std=c++11


