TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui
LIBS += `pkg-config opencv --libs`
SOURCES += picture2.cpp \
    visionmethods.cpp

HEADERS += \
    visionmethods.h

