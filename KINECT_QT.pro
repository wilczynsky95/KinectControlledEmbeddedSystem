QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KINECT_QT
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
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/include/ni
INCLUDEPATH += inc
# OpenCV Libs
LIBS += -lopencv_imgproc -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_objdetect -lopencv_video -lOpenNI


SOURCES += \
        src/main.cpp \
        src/Mainwindow.cpp \
        src/Viewer.cpp \
        src/DepthTracker.cpp \
        src/SerialPort.cpp

HEADERS += \
        inc/Mainwindow.h \
        inc/Viewer.h \
        inc/DepthTracker.h \
        inc/SerialPort.h

FORMS += \
        Mainwindow.ui
