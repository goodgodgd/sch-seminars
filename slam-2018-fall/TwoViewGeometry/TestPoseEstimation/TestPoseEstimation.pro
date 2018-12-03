TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

# equivalent to #define PRJ_PATH path/to/QtApp.pro
DEFINES += PRJ_PATH=\\\"$$PWD\\\"

LIBRARY_ROOT=/home/ian/mylib/deploy

# import opencv
OPENCV_VERSION = opencv-3.4.4
INCLUDEPATH += $$LIBRARY_ROOT/$$OPENCV_VERSION/include
LIBS += -L$$LIBRARY_ROOT/$$OPENCV_VERSION/lib
CV_LIB_FULL = $$system("find $$LIBRARY_ROOT/$$OPENCV_VERSION/lib -maxdepth 1 -name '*.so'")
for(eachlib, CV_LIB_FULL):CV_LIB_SO+=$$replace(eachlib, $$LIBRARY_ROOT/$$OPENCV_VERSION/lib/libopencv, -lopencv)
for(eachlib, CV_LIB_SO):CV_LIB+=$$replace(eachlib, .so, )
message($$CV_LIB)
LIBS += $$CV_LIB


SOURCES += \
        main.cpp
