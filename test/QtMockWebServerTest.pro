#-------------------------------------------------
#
# Project created by QtCreator 2014-10-12T10:52:05
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = RecordedRequestTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
QMAKE_MAC_SDK = macosx10.9

include(../src/QtMockWebServer.pri)

INCLUDEPATH += \
    ../src/

SOURCES += \
    main.cpp \
    TestRecordedRequest.cpp \
    TestMockResponse.cpp \
    TestQtMockWebServer.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    TestRecordedRequest.h \
    TestMockResponse.h \
    TestQtMockWebServer.h
