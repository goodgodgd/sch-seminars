TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBRARY_ROOT=/home/ian/mylib/deploy

# import opencv
INCLUDEPATH += $$LIBRARY_ROOT/opencv-3.4.1/include
LIBS += -L$$LIBRARY_ROOT/opencv-3.4.1/lib

CV_LIB_FULL = $$system("find $$LIBRARY_ROOT/opencv-3.4.1/lib -maxdepth 1 -name '*.so'")
for(eachlib, CV_LIB_FULL):CV_LIB_SO+=$$replace(eachlib, $$LIBRARY_ROOT/opencv-3.4.1/lib/libopencv, -lopencv)
for(eachlib, CV_LIB_SO):CV_LIB+=$$replace(eachlib, .so, )
message($$CV_LIB)
LIBS += $$CV_LIB


SOURCES += \
        main.cpp

HEADERS += \
    keypointdetector.h
