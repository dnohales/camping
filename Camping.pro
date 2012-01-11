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
    DialogPrintClients.cpp \
    DialogReceiptEdit.cpp \
    RichTextEditorWidget.cpp

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
    DialogPrintClients.h \
    DialogReceiptEdit.h \
    RichTextEditorWidget.h

FORMS    += MainWindow.ui \
    FrameTents.ui \
    FrameDorms.ui \
    DialogClient.ui \
    DialogClientSelector.ui \
    DialogAbout.ui \
    DialogPrintClients.ui \
    DialogReceiptEdit.ui

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
    html/ReceiptDefault.html \
    html/ClientCollectionDefault.html \
    images/tent.png \
    images/search.svg \
    images/red-arrow.svg \
    images/open.svg \
    images/new.svg \
    images/green-arrow.svg \
    images/dorm.png \
    images/delete.svg \
    imgs/tent.png \
    imgs/search.svg \
    imgs/red-arrow.svg \
    imgs/open.svg \
    imgs/new.svg \
    imgs/green-arrow.svg \
    imgs/dorm.png \
    imgs/delete.svg \
    imgs/add.svg \
    imgs/richtexteditor/textunder.png \
    imgs/richtexteditor/textsuperscript.png \
    imgs/richtexteditor/textsubscript.png \
    imgs/richtexteditor/textright.png \
    imgs/richtexteditor/textleft.png \
    imgs/richtexteditor/textjustify.png \
    imgs/richtexteditor/textitalic.png \
    imgs/richtexteditor/textcenter.png \
    imgs/richtexteditor/textbold.png
