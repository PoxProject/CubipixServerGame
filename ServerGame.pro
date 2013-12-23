#-------------------------------------------------
#
# Project created by QtCreator 2013-12-08T09:38:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerGame
TEMPLATE = app


SOURCES += main.cpp\
        servergame.cpp \
    player.cpp \
    part.cpp

HEADERS  += servergame.h \
    player.h \
    part.h

FORMS    += servergame.ui
