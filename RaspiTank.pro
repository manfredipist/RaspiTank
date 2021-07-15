QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PiBus/qi2c.cpp \
    PiController/picontroller.cpp \
    PiDevices/PiCamera/picamera.cpp \
    PiDevices/PiI2CMultiplexer/pitca9548a.cpp \
    PiDevices/PiMotors/pifan.cpp \
    PiDevices/PiMotors/piservomotor.cpp \
    PiDevices/PiMotors/pisteppermotor.cpp \
    PiDevices/PiSensors/pihcsr04.cpp \
    PiDevices/PiSensors/pimpu6050.cpp \
    PiDevices/PiSensors/piqmc5883l.cpp \
    PiDevices/PiSensors/pitof10120.cpp \
    PiDevices/PiSensors/pivl53l0x.cpp \
    PiDevices/PiSensors/pivl53l1x.cpp \
    PiDevices/PiPWMController/pipca9685.cpp \
    PiDevices/PiSensors/pivl6180x.cpp \
    PiThreads/PiMotion/pimotion.cpp \
    PiThreads/PiPlanner/piplanner.cpp \
    PiThreads/PiSensing/pisensing.cpp \
    PiThreads/PiVision/pivision.cpp \
    PiWidgets/PiMap/pimap.cpp \
    PiWidgets/PiCameraController/picameracontroller.cpp \
    main.cpp

HEADERS += \
    PiBus/qi2c.h \
    PiController/picontroller.h \
    PiDevices/PiCamera/picamera.h \
    PiDevices/PiI2CMultiplexer/pitca9548a.h \
    PiDevices/PiMotors/pifan.h \
    PiDevices/PiMotors/piservomotor.h \
    PiDevices/PiMotors/pisteppermotor.h \
    PiDevices/PiSensors/pihcsr04.h \
    PiDevices/PiSensors/pimpu6050.h \
    PiDevices/PiSensors/piqmc5883l.h \
    PiDevices/PiSensors/pitof10120.h \
    PiDevices/PiSensors/pivl53l0x.h \
    PiDevices/PiSensors/pivl53l1x.h \
    PiDevices/PiPWMController/pipca9685.h \
    PiDevices/PiSensors/pivl6180x.h \
    PiThreads/PiMotion/pimotion.h \
    PiThreads/PiPlanner/piplanner.h \
    PiThreads/PiSensing/pisensing.h \
    PiThreads/PiVision/pivision.h \
    PiWidgets/PiMap/pimap.h \
    PiWidgets/PiCameraController/picameracontroller.h


FORMS += \
    PiController/picontroller.ui \
    PiWidgets/PiMap/pimap.ui \
    PiWidgets/PiCameraController/picameracontroller.ui

# Default rules for deployment.
qnx: target.path = /home/pi/$${TARGET}
else: unix:!android: target.path = /home/pi/$${TARGET}
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../../../../../opt/rpi/sysroot/usr/lib/ -lpigpiod_if2
INCLUDEPATH += $$PWD/../../../../../opt/rpi/sysroot/usr/include
DEPENDPATH += $$PWD/../../../../../opt/rpi/sysroot/usr/include

unix:!macx: LIBS += -L$$PWD/../../../../../opt/rpi/sysroot/opt/vc/lib/ -lmmal -lmmal_core -lmmal_vc_client -lmmal_components -lmmal_util -lvchiq_arm -lvcsm -lcontainers -lvcos -lbcm_host
INCLUDEPATH += $$PWD/../../../../../opt/rpi/sysroot/opt/vc/lib/
DEPENDPATH += $$PWD/../../../../../opt/rpi/sysroot/opt/vc/lib/

unix:!macx: LIBS += -L$$PWD/../../../../../opt/rpi/sysroot/usr/local/lib/ -lopencv_core -lopencv_flann -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_photo -lopencv_videoio
INCLUDEPATH += $$PWD/../../../../../opt/rpi/sysroot/usr/local/include/opencv4
DEPENDPATH += $$PWD/../../../../../opt/rpi/sysroot/usr/local/include/opencv4

unix:!macx: LIBS += -L$$PWD/../../../../../opt/rpi/sysroot/usr/local/lib/ -lraspicam -lraspicam_cv
INCLUDEPATH += $$PWD/../../../../../opt/rpi/sysroot/usr/local/include
DEPENDPATH += $$PWD/../../../../../opt/rpi/sysroot/usr/local/include

RESOURCES += \
    resource.qrc

DISTFILES += \
    Icons/camera.png \
    Icons/gear.png \
    Icons/map.png


