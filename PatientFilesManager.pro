QT       += core gui
QT += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AddNewPatDialog.cpp \
    FileMonitorWidget.cpp \
    GlobalVar.cpp \
    ImageViewerWidget.cpp \
    MessDispalyWidget.cpp \
    ModifyPatDialog.cpp \
    MyListWidget.cpp \
    PatientDataBase.cpp \
    PhotoWidget.cpp \
    Tool.cpp \
    main.cpp \
    patient.cpp

HEADERS += \
    AddNewPatDialog.h \
    FileMonitorWidget.h \
    GlobalVar.h \
    ImageViewerWidget.h \
    MessDispalyWidget.h \
    ModifyPatDialog.h \
    MyListWidget.h \
    PatientDataBase.h \
    PhotoWidget.h \
    Tool.h \
    patient.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icon/dir.jpg \
    icon/file.jpg \
    icon/pdf.jpg

RESOURCES += \
    images.qrc
