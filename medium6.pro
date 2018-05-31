QT += core
QT -= gui

CONFIG += c++11

TARGET = medium6
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    CppSQLite3.cpp \
    sqlite3.c

HEADERS += \
    CppSQLite3.h \
    sqlite3.h
