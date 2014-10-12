#-------------------------------------------------
#
# Project created by QtCreator 2014-10-08T21:22:33
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = QtMockWebServer
TEMPLATE = lib
CONFIG += staticlib
QMAKE_MAC_SDK = macosx10.9

include(QtMockWebServer.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}
