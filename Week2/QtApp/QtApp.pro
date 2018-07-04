#-------------------------------------------------
#
# Project created by QtCreator 2018-06-26T17:37:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtApp
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

## study point: c++11 ??
# enable c++11
CONFIG += c++14

# equivalent to #define PRJ_PATH path/to/QtApp.pro
DEFINES += PRJ_PATH=\\\"$$PWD\\\"

# this does not need here
INCLUDEPATH += $$PWD

# set library path that was built by your hand
LIBRARY_ROOT=/home/ian/mylib/deploy
# when use "#include", compiler searches INCLUDEPATH
INCLUDEPATH += $$LIBRARY_ROOT/opencv-3.4.1/include
# add library path to find *.so
LIBS += -L$$LIBRARY_ROOT/opencv-3.4.1/lib

# import opencv manually
LIBS += -lopencv_core   \
    -lopencv_imgproc    \
    -lopencv_highgui    \
    -lopencv_imgcodecs

## study point: qmake script
## when importing tens or hundreds of library files, import them like this
#CV_LIB_FULL = $$system("find $$LIBRARY_ROOT/opencv-3.4.1/lib -maxdepth 1 -name '*.so'")
#for(eachlib, CV_LIB_FULL):CV_LIB_SO+=$$replace(eachlib, $$LIBRARY_ROOT/opencv-3.4.1/lib/libopencv, -lopencv)
#for(eachlib, CV_LIB_SO):CV_LIB+=$$replace(eachlib, .so, )
#message($$CV_LIB)
#LIBS += $$CV_LIB

SOURCES += \
        main.cpp \
        myqtapp.cpp

HEADERS += \
        myqtapp.h \
    shapedrawer.h \
    paramparser.h \
    multishapedrawer.h

FORMS += \
        myqtapp.ui
