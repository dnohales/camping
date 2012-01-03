#-------------------------------------------------
#
# Project created by QtCreator 2011-12-29T16:46:37
#
#-------------------------------------------------

QT       += core gui sql

TARGET = Camping
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    FrameTents.cpp \
    FrameDorms.cpp \
    CampingApplication.cpp \
    CampingConfig.cpp

HEADERS  += MainWindow.h \
    FrameTents.h \
    FrameDorms.h \
    CampingApplication.h \
    CampingConfig.h \
    main.h

FORMS    += MainWindow.ui \
    FrameTents.ui \
    FrameDorms.ui

RESOURCES += \
    default.qrc
