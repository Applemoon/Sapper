QT       += core gui

TARGET = Sapper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tcell.cpp \
    fieldoptionsdialog.cpp

HEADERS  += mainwindow.h \
    tcell.h \
    fieldoptionsdialog.h

FORMS    += mainwindow.ui \
    fieldoptionsdialog.ui

RESOURCES += \
    res.qrc

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
  DEFINES += HAVE_QT5
}

QMAKE_CXXFLAGS += -Wall -Werror -Wextra
