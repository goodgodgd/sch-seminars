#-------------------------------------------------
#
# Project created by QtCreator 2018-07-16T15:55:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MotionTracking
TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# equivalent to #define PRJ_PATH path/to/QtApp.pro
DEFINES += PRJ_PATH=\\\"$$PWD\\\"

LIBRARY_ROOT=/home/ian/mylib/deploy

# import opencv
OPENCV_VERSION = opencv-3.4
INCLUDEPATH += $$LIBRARY_ROOT/$$OPENCV_VERSION/include
LIBS += -L$$LIBRARY_ROOT/$$OPENCV_VERSION/lib
CV_LIB_FULL = $$system("find $$LIBRARY_ROOT/$$OPENCV_VERSION/lib -maxdepth 1 -name '*.so'")
for(eachlib, CV_LIB_FULL):CV_LIB_SO+=$$replace(eachlib, $$LIBRARY_ROOT/$$OPENCV_VERSION/lib/libopencv, -lopencv)
for(eachlib, CV_LIB_SO):CV_LIB+=$$replace(eachlib, .so, )
message($$CV_LIB)
LIBS += $$CV_LIB


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Modules/glvertexmanager.cpp \
    Modules/glwidget.cpp \
    Modules/motiontracker.cpp \
    Modules/deschandler.cpp

HEADERS += \
        mainwindow.h \
    Modules/glvertexmanager.h \
    Modules/glwidget.h \
    Modules/pose6dof.h \
    Modules/project_common.h \
    Modules/pose6dofqt.h \
    Modules/motiontracker.h \
    Modules/deschandler.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    Modules/fragment.frag \
    Modules/vertex.vert
