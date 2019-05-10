TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

# equivalent to #define PRJ_PATH path/to/QtApp.pro
DEFINES += PRJ_PATH=\\\"$$PWD\\\"

LIBRARY_ROOT=/home/ian/mylib/deploy

# import suitesparse
INCLUDEPATH += /usr/include/suitesparse
LIBS += -lsuitesparseconfig -lcholmod -lcxsparse

# import g2o
INCLUDEPATH += $$LIBRARY_ROOT/g2o/include
INCLUDEPATH += $$LIBRARY_ROOT/g2o/include/g2o/EXTERNAL/csparse

LIBS += -L$$LIBRARY_ROOT/g2o/lib
# search .so files
G2O_LIB_FULL = $$system("find $$LIBRARY_ROOT/g2o/lib -maxdepth 1 -name '*.so'")
# remove file paths
for(eachlib, G2O_LIB_FULL):G2O_LIB1+=$$replace(eachlib, $$LIBRARY_ROOT/g2o/lib/libg2o_, -lg2o_)
# solver -> tolver
for(eachlib, G2O_LIB1):G2O_LIB2+=$$replace(eachlib, solver, tolver)
# remove extension: ".so" -> ""		: this line replace "_solver" to "lver"
for(eachlib, G2O_LIB2):G2O_LIB3+=$$replace(eachlib, .so, )
# roll back tolver -> solver
for(eachlib, G2O_LIB3):G2O_LIB4+=$$replace(eachlib, tolver, solver)
message("g2o library files": $$G2O_LIB4)
LIBS += $$G2O_LIB4

# import eigen
INCLUDEPATH += /usr/include/eigen3


SOURCES += \
        main.cpp
