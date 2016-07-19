#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T10:41:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HypCam
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    __common.cpp \
    hypCamAPI.cpp \
    graphicsview.cpp \
    customline.cpp \
    selcolor.cpp \
    customrect.cpp \
    showancalchrres.cpp \
    gencalibxml.cpp \
    rotationfrm.cpp \
    recparamfrm.cpp \
    generatehypercube.cpp \
    validatecalibration.cpp

HEADERS  += mainwindow.h \
    __common.h \
    lstStructs.h \
    hypCamAPI.h \
    graphicsview.h \
    customline.h \
    selcolor.h \
    customrect.h \
    showancalchrres.h \
    gencalibxml.h \
    rotationfrm.h \
    recparamfrm.h \
    generatehypercube.h \
    validatecalibration.h

FORMS    += mainwindow.ui \
    selcolor.ui \
    showancalchrres.ui \
    gencalibxml.ui \
    rotationfrm.ui \
    recparamfrm.ui \
    generatehypercube.ui \
    validatecalibration.ui

RESOURCES += \
    lstImgs.qrc



INCLUDEPATH += /usr/local/include/opencv
#LIBS += -L/usr/local/lib
LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_calib3d

#LIBS += -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d
#LIBS += -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching

# -lopencv_features2d  -lopencv_flann -lopencv_legacy -lopencv_nonfree -lopencv_photo -lopencv_superres -lopencv_ts
# -lopencv_videostab

