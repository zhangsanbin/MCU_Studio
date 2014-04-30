#-------------------------------------------------
#
# Project created by QtCreator 2014-01-29T10:53:26
#
#-------------------------------------------------

QT       += core gui
QT += widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MCU_Studio
TEMPLATE = app


SOURCES += main.cpp\
    mcu_studio.cpp \
    File.cpp \
    highlighter.cpp \
    com/qextserialbase.cpp \
    com/posix_qextserialport.cpp \
    com/com.cpp \
    builder.cpp

HEADERS  += \
    mcu_studio.h \
    database.h \
    File.h \
    highlighter.h \
    com/qextserialbase.h \
    com/posix_qextserialport.h \
    com/com.h \
    builder.h

FORMS    += \
    mcu_studio.ui \
    com/com.ui

RESOURCES += \
    Res.qrc

RC_FILE = myapp.rc
