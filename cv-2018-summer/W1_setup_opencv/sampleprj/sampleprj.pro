#-------------------------------------------------
#
# Project created by QtCreator 2018-06-20T21:47:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sampleprj
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

message($$dirname(PWD))
DEFINES += PROJECT_PATH=\\\"$$dirname(PWD)\\\"

LIBRARY_ROOT=/home/ian/mylib/deploy

# import opencv
INCLUDEPATH += $$LIBRARY_ROOT/opencv-3.4.1/include
LIBS += -L$$LIBRARY_ROOT/opencv-3.4.1/lib    \
    -lopencv_core   \
    -lopencv_imgproc    \
    -lopencv_highgui    \
    -lopencv_imgcodecs

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
