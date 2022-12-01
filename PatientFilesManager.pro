QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FileClassBase.cpp \
    MainWidget.cpp \
    Tool.cpp \
    add_new_patient_box.cpp \
    infobase.cpp \
    main.cpp \
    patient.cpp

HEADERS += \
    FileClassBase.h \
    MainWidget.h \
    Tool.h \
    add_new_patient_box.h \
    infobase.h \
    patient.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
