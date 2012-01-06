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
    CampingConfig.cpp \
    CampingException.cpp \
    ActiveRecord.cpp \
    Client.cpp \
    Location.cpp \
    SqlCriteria.cpp \
    Vehicle.cpp

HEADERS  += MainWindow.h \
    FrameTents.h \
    FrameDorms.h \
    CampingApplication.h \
    CampingConfig.h \
    main.h \
    CampingException.h \
    ActiveRecord.h \
    Client.h \
    Location.h \
    SqlCriteria.h \
    common.h \
    Vehicle.h

FORMS    += MainWindow.ui \
    FrameTents.ui \
    FrameDorms.ui

RESOURCES += \
    default.qrc

OTHER_FILES += \
    template.sql
