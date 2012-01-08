#-------------------------------------------------
#
# Project created by QtCreator 2011-12-29T16:46:37
#
#-------------------------------------------------

QT       += core gui sql webkit

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
    Vehicle.cpp \
    DialogClient.cpp \
    MainFrame.cpp \
    DialogClientSelector.cpp \
    DialogAbout.cpp \
    DialogPrintClients.cpp

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
    Vehicle.h \
    DialogClient.h \
    MainFrame.h \
    DialogClientSelector.h \
    DialogAbout.h \
    DialogPrintClients.h

FORMS    += MainWindow.ui \
    FrameTents.ui \
    FrameDorms.ui \
    DialogClient.ui \
    DialogClientSelector.ui \
    DialogAbout.ui \
    DialogPrintClients.ui

RESOURCES += \
    default.qrc

OTHER_FILES += \
    template.sql \
    search.svg \
    open.svg \
    new.svg \
    tent.png \
    red-arrow.svg \
    green-arrow.svg \
    dorm.png \
    ReceiptDefault.html \
    ClientCollectionDefault.html
