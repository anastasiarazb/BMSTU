TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/rasterization.cpp \
    src/callbacks.cpp \
    src/fill.cpp
INCLUDEPATH += ../../glm/glm
LIBS += -lGL -lGLU -lGLEW -lglfw

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    src/rasterization.h \
    src/callbacks.h
