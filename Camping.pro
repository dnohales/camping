#-------------------------------------------------
#
# Project created by QtCreator 2011-12-29T16:46:37
#
#-------------------------------------------------

QT       += core gui widgets printsupport sql webenginewidgets

TARGET = Camping
TEMPLATE = app


SOURCES += src/main.cpp\
    src/MainWindow.cpp \
    src/FrameTents.cpp \
    src/FrameDorms.cpp \
    src/CampingApplication.cpp \
    src/CampingConfig.cpp \
    src/CampingException.cpp \
    src/ActiveRecord.cpp \
    src/Client.cpp \
    src/Location.cpp \
    src/SqlCriteria.cpp \
    src/Vehicle.cpp \
    src/DialogClient.cpp \
    src/MainFrame.cpp \
    src/DialogClientSelector.cpp \
    src/DialogAbout.cpp \
    src/DialogPrintClients.cpp \
    src/DialogReceiptEdit.cpp \
    src/RichTextEditorWidget.cpp

HEADERS += src/MainWindow.h \
    src/FrameTents.h \
    src/FrameDorms.h \
    src/CampingApplication.h \
    src/CampingConfig.h \
    src/main.h \
    src/CampingException.h \
    src/ActiveRecord.h \
    src/Client.h \
    src/Location.h \
    src/SqlCriteria.h \
    src/common.h \
    src/Vehicle.h \
    src/DialogClient.h \
    src/MainFrame.h \
    src/DialogClientSelector.h \
    src/DialogAbout.h \
    src/DialogPrintClients.h \
    src/DialogReceiptEdit.h \
    src/RichTextEditorWidget.h

FORMS += src/ui/MainWindow.ui \
    src/ui/FrameTents.ui \
    src/ui/FrameDorms.ui \
    src/ui/DialogClient.ui \
    src/ui/DialogClientSelector.ui \
    src/ui/DialogAbout.ui \
    src/ui/DialogPrintClients.ui \
    src/ui/DialogReceiptEdit.ui

RESOURCES += \
    res/default.qrc

OTHER_FILES += \
    res/db/template.sql \
    res/html/receipt.html \
    res/html/client-collection.html \
    res/imgs/tent.png \
    res/imgs/search.svg \
    res/imgs/red-arrow.svg \
    res/imgs/open.svg \
    res/imgs/new.svg \
    res/imgs/green-arrow.svg \
    res/imgs/dorm.png \
    res/imgs/delete.svg \
    res/imgs/add.svg

RC_FILE = camping.rc
