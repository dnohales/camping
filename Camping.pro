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
    src/MainFrame.cpp \
    src/DialogAbout.cpp \
    src/DialogReceiptEdit.cpp \
    src/RichTextEditorWidget.cpp \
    src/Reservation.cpp \
    src/DialogReservation.cpp \
    src/DialogReservationSelector.cpp \
    src/DialogPrintReservations.cpp

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
    src/MainFrame.h \
    src/DialogAbout.h \
    src/DialogReceiptEdit.h \
    src/RichTextEditorWidget.h \
    src/Reservation.h \
    src/DialogReservation.h \
    src/DialogReservationSelector.h \
    src/DialogPrintReservations.h

FORMS += src/ui/MainWindow.ui \
    src/ui/FrameTents.ui \
    src/ui/FrameDorms.ui \
    src/ui/DialogAbout.ui \
    src/ui/DialogReceiptEdit.ui \
    src/ui/DialogReservation.ui \
    src/ui/DialogReservationSelector.ui \
    src/ui/DialogPrintReservations.ui

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
