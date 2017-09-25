#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T11:05:52
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScenarioDesigner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myview.cpp \
    myscene.cpp \
    marker.cpp \
    markerwindow.cpp \
    scenario.cpp \
    styles.cpp \
    map.cpp \
    dialognewitem.cpp \
    markerlink.cpp

HEADERS  += mainwindow.h \
    myview.h \
    myscene.h \
    marker.h \
    markerwindow.h \
    scenario.h \
    styles.h \
    MyEnums.h \
    map.h \
    dialognewitem.h \
    markerlink.h

FORMS    += mainwindow.ui \
    markerwindow.ui \
    dialognewitem.ui

RESOURCES += \
    Images.qrc
